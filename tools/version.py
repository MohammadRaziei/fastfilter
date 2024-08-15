from __future__ import annotations

import re
import sys
from pathlib import Path


class VersionManager:
    def __init__(self, file_path):
        self.file_path = file_path
        self.load_file()

    def load_file(self):
        """Load the file content into memory."""
        with self.file_path.open("r") as file:
            self.content = file.read()

    def save_file(self):
        """Save the content back to the file using the system's native line separator."""
        with self.file_path.open("w") as file:
            file.write(self.content)  # Write the content as it is, preserving line endings

    def get_version(self):
        """Get the current version from the text."""
        match = re.search(r'version = "(.*?)"', self.content)
        if match:
            return match.group(1)
        return None

    def set_version(self, new_version):
        """Set a new version in the text."""
        new_version = new_version.lstrip('v')  # Remove 'v' prefix if present
        assert self.is_valid_version(new_version)
        self.content = re.sub(r'version = ".*"', f'version = "{new_version}"', self.content, count=1)
        self.save_file()

    @staticmethod
    def is_valid_version(version_str):
        """
        Check if the given string is a valid version number according to SemVer.
        Parameters:
        - version_str (str): The version string to check.
        Returns:
        - bool: True if the version string is valid, False otherwise.
        """
        pattern = r'^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-(0|[1-9]\d*|[\da-z-]+(?:\.(0|[1-9]\d*|[\da-z-]+)+)*))?(?:\+[0-9A-Za-z-]+(\.[0-9A-Za-z-]+)*)?$'
        return bool(re.match(pattern, version_str))


if __name__ == "__main__":
    pyproject_path = Path(__file__).parent.parent / "pyproject.toml"
    version_manager = VersionManager(pyproject_path)

    if len(sys.argv) > 1:
        # An argument was passed, assume it's a new version number
        new_version = sys.argv[1]
        old_version = version_manager.get_version()
        version_manager.set_version(new_version)
        sys.stdout.write(f"Version changed from \"{old_version}\" to \"{new_version.lstrip('v')}\".\n")
    else:
        # No arguments passed, print the current version
        current_version = version_manager.get_version()
        sys.stdout.write(f"Current version: \"{current_version}\"\n")
