# Server_Proxy_Cache_Hub

## Important Note

Please note that some code segments are intentionally redacted in compliance with school policy. This information is shared for the purpose of demonstrating technical skills to potential employers and should not be publicly disclosed. Please be advised that this repository may only be available temporarily. Prospective employers are welcome to request access to the comprehensive and executable codebase directly from the author.

Only files that showcase substantial contributions from the authors are included. Furthermore, the documentation intentionally omits detailed descriptions of functions and maintains a general project structure in this public repository. This approach is adopted to uphold the integrity of the instructional staff's efforts and prevent unauthorized duplication of the project.

## Project Overview

1. **Part 1** Converts a getfile server into a proxy server. This server will accept incoming GETFILE requests and translate them into http requests for another server, such as one located on the internet.
2. **Part 2** Implements a simple cache server that communicates with the proxy via **shared memory**. The design of this cache server is intended to permit its use with **multiple** proxy servers.

## Project Approach

Throughout the project, the author has recorded approaches to each part of the project and the resources used. This can be made available upon request to the author.


## Part 1

The initial code comes equipped with a comprehensive gfserver library, effectively implementing the HTTP server's functionality. In Part 1, the sole requirement is to register a callback, and this can be seamlessly achieved by employing [libcurl's “easy” C interface](http://curl.haxx.se/libcurl/c/).

## Part 2

Implemented a cache process that will run on the same machine as the proxy and communicate with it via shared memory.

For transferring local files efficiently, 2 inter-process communication channels were maintained: data channel and command channel. File content was transferred through the data channel and implemented using shared memory. Transfer commands were communicated through the command channel and implemented using other IPC mechanisms, such as a message queue.

The cache implementation was abstracted away through a provided API. The focus of part 2 was on relaying the contents of the
cache to the proxy process by way of shared memory. Instead of automatically relaying requests to the http server, the proxy queried the cache to see if the file is available in local memory already.  If the file is available there, it sent the cached version.

The proxy was responsible for managing the shared memory to ensure:
- a non-responsive client doesn’t degrade the performance of the cache
- a malicious client doesn’t corrupt another’s data.

The cache set up some communication mechanism by which the proxy can communicate its request along with the information about the communication mechanism.

The proxy process used a signal handler for both SIGTERM (signal for kill) and SIGINT (signal for Ctrl-C) to first remove all existing IPC objects -- shared memory segments, semaphores, message queues, or anything else -- and only then terminate. Same guidelines applied for the cache client process.