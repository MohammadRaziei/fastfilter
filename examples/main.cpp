//
// Created by mohammad on 5/20/23.
//
#include <iostream>
#include "common.h"
#include "medianFilter.h"



// HTTPS (Hypertext Transfer Protocol Secure): A secure version of HTTP that
// encrypts data transmission to prevent unauthorized access. URLs that begin
// with "https://" are used for accessing secure websites. FTP: (File Transfer
// Protocol): Used for transferring files between computers on a network. URLs
// that begin with "ftp://" are used for accessing files on an FTP server. SSH:
// (Secure Shell): Used for secure remote access to a computer or server. URLs
// that begin with "ssh://" are used for accessing a remote computer or server
// via SSH. SMTP: (Simple Mail Transfer Protocol): Used for sending email
// messages between servers. URLs that begin with "smtp://" are used for sending
// email messages. IMAP: (Internet Message Access Protocol): Used for retrieving
// email messages from a server. URLs that begin with "imap://" are used for
// accessing email messages. POP: (Post Office Protocol): Used for retrieving
// email messages from a server. URLs that begin with "pop://" are used for
// accessing email messages.

//inline float myrand() { return (float)std::rand() / (RAND_MAX); }
inline float myrand() { return float(std::rand() % 20); }

int main() {
    std::vector<float> data(100);
    std::srand(0);
    std::generate(data.begin(), data.end(),
                  []() { return myrand() < 0.001 ? 100 * myrand() : myrand(); });

    show(data);
    std::vector<float> filtData(data.size());
    const uint32_t halfWindowSize = 5;
    tic;
    utils::movingFilter(filtData, data, halfWindowSize, utils::medianFilterKernel);
    toc;

    show(filtData);
    printf("\ngood bye :)\n");
    return 0;

}
