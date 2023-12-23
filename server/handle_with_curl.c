#include "proxy-student.h"
#include "gfserver.h"

#define MAX_REQUEST_N 822
#define BUFSIZE (6200)

ssize_t handle_with_curl(gfcontext_t *ctx, const char *path, void* arg) {
    // Construct complete URL
    char full_url[MAX_REQUEST_N];
    const char *base = (const char *)arg;
    if (strstr(base, "localhost")) {
        snprintf(full_url, sizeof(full_url), "http://%s%s", base, path);
    } else {
        snprintf(full_url, sizeof(full_url), "%s%s", base, path);
    }
	
    CURL *curl = curl_easy_init();
    if (!curl) {
        perror("curl easy init");
        return -1;
    }

    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, full_url);

    // Redacted extracting request metadata

    // Redacted part of request validation

    if (response_code == 404) {
        gfs_sendheader(ctx, GF_FILE_NOT_FOUND, 0);
        curl_easy_cleanup(curl);
        return GF_FILE_NOT_FOUND;
    }

    // Fetch file size
    long content_length;
    // Redacted retrieving file size
    gfs_sendheader(ctx, GF_OK, (size_t)content_length);
    ctx->file_len = content_length;

    // Fetch the file content
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, ctx);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        gfs_sendheader(ctx, GF_ERROR, 0);
        return SERVER_FAILURE;
    }
    return GF_OK;	
}