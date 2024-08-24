from __future__ import annotations

import os
import sys


def make_relative_paths(base_path, includes):
    base_path = os.path.abspath(base_path)
    includes_list = includes.split(';')
    relative_paths = [os.path.relpath(inc, base_path) for inc in includes_list if inc.startswith(base_path)]
    return relative_paths

def list_to_str(lists, prefix=" "):
    return prefix + prefix.join(lists) if len(lists) > 0 else " "


if __name__ == "__main__":
    name = sys.argv[1]
    base_path = sys.argv[2]
    includes = sys.argv[3]
    sources = sys.argv[4]
    relative_includes = make_relative_paths(base_path, includes)
    relative_sources = make_relative_paths(base_path, sources)
    includes_str = list_to_str(relative_includes, " -I")
    sources_str = list_to_str(relative_sources)
    sys.stdout.write("mex" + includes_str + sources_str + f" -output {name}")




