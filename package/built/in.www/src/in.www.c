/*
 
 in.www.c
 Copyright(c)2008, R. Rawson-Tetley

 Simple, static HTTP server for use with inetd and my
 NSLU2 (slug) - I use this for serving mp3s and CGI
 handling.

 Supports static content, GET/POST requests and CGI.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License as
 published by the Free Software Foundation; either version 2 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the
 Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston
 MA 02111-1307, USA.

 Contact me by electronic mail: bobintetley@users.sourceforge.net
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <netinet/in.h>
#include <fcntl.h>

#ifdef LOGGING
#include <syslog.h>
#endif

#define SERVER "in.www/200809"
#define DEFAULT_MIMETYPE "text/plain"
#define PROTOCOL "HTTP/1.0"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"
#define COMBINEDFMT "%d/%b/%Y:%H:%M:%S %z"
#define LOGFILE "/var/log/in.www.log"

// Output buffer size for CGI calls
#define CGI_BUFFER 262140
// Buffer size for each CGI environment variable and input buffer
#define TOKEN_SIZE 4096

#define STDIN 0
#define READ 0
#define WRITE 1

// The client request
char request[TOKEN_SIZE];

// Whether the stream is exhausted after reading the HTTP headers
int exhausted = 0;

char* get_mime_type(char* name)
{
    // This is really crude, but quick. Could probably use
    // libmagic for accuracy
    char* ext = strrchr(name, '.');
    if (!ext) return DEFAULT_MIMETYPE;
    if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0 || strcmp(ext, ".JPG") == 0) return "image/jpeg";
    if (strcmp(ext, ".txt") == 0) return "text/plain";
    if (strcmp(ext, ".csv") == 0) return "text/csv";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".au") == 0) return "audio/basic";
    if (strcmp(ext, ".wav") == 0) return "audio/wav";
    if (strcmp(ext, ".avi") == 0) return "video/x-msvideo";
    if (strcmp(ext, ".mpeg") == 0 || strcmp(ext, ".mpg") == 0) return "video/mpeg";
    if (strcmp(ext, ".mp3") == 0) return "audio/mpeg";
    
    #ifdef CGI
    if (strcmp(ext, ".cgi") == 0) return "cgi";
    if (strcmp(ext, ".pl") == 0) return "cgi";
    if (strcmp(ext, ".lua") == 0) return "cgi";
    if (strcmp(ext, ".php") == 0) return "cgi";
    if (strcmp(ext, ".sh") == 0) return "cgi";
    #else
    if (strcmp(ext, ".cgi") == 0) return "text/plain";
    #endif

    return DEFAULT_MIMETYPE;
}

#ifdef ONELOG
// Simple logger, outputs in combined format
void logrequest(int code, int contentlen) {

    // Get the IP address
    struct sockaddr_in from;
    int fromlen = sizeof(from);
    memset(&from, 0, sizeof(from));
    getpeername(STDIN, (struct sockaddr *) &from, &fromlen);

    // Generate the log date
    char timebuf[128];
    time_t now;
    now = time(NULL);
    strftime(timebuf, sizeof(timebuf), COMBINEDFMT, localtime(&now));

    // Content length
    char clen[10];
    if (contentlen < 0)
        strcpy(clen, "-");
    else
        sprintf(clen, "%d", contentlen);

    // Refer and user agent
    char* httpref = getenv("HTTP_REFERER");
    char* httpua = getenv("HTTP_USER_AGENT");
    char referer[TOKEN_SIZE];
    char useragent[TOKEN_SIZE];
    if (httpref) 
        strcpy(referer, httpref);
    else
        strcpy(referer, "-");
    if (httpua) 
        strcpy(useragent, httpua);
    else
        strcpy(useragent, "-");


    // Write to the file
    FILE* f = fopen(LOGFILE, "a");
    fprintf(f, "%s - - [%s] \"%s\" %d %s \"%s\" \"%s\"\n", inet_ntoa(from.sin_addr), timebuf, request, code, clen, referer, useragent);
    fclose(f);

}
#endif

// Bidirectional version of popen using pipe duplication
pid_t popen2(const char *command, int *infp, int *outfp)
{
    int p_stdin[2], p_stdout[2];
    pid_t pid;

    if (pipe(p_stdin) != 0 || pipe(p_stdout) != 0)
        return -1;

    pid = fork();

    if (pid < 0)
        return pid;
    else if (pid == 0)
    {
        close(p_stdin[WRITE]);
        dup2(p_stdin[READ], READ);
        close(p_stdout[READ]);
        dup2(p_stdout[WRITE], WRITE);
        execl("/bin/sh", "sh", "-c", command, NULL);
        perror("execl");
        exit(1);
    }

    if (infp == NULL)
        close(p_stdin[WRITE]);
    else
        *infp = p_stdin[WRITE];

    if (outfp == NULL)
        close(p_stdout[READ]);
    else
        *outfp = p_stdout[READ];

    close(p_stdin[READ]);
    close(p_stdout[WRITE]);
    return pid;
}

void send_headers(int status, char* title, char* extra, char* mime, 
        int length, time_t date, int complete)
{
    time_t now;
    char timebuf[128];

    printf("%s %d %s\r\n", PROTOCOL, status, title);
    printf("Server: %s\r\n", SERVER);
    now = time(NULL);
    strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&now));
    printf("Date: %s\r\n", timebuf);
    if (extra) printf("%s\r\n", extra);
    if (mime) printf("Content-Type: %s\r\n", mime);
    if (length >= 0) printf("Content-Length: %d\r\n", length);
    if (date != -1)
    {
        strftime(timebuf, sizeof(timebuf), RFC1123FMT, gmtime(&date));
        printf("Last-Modified: %s\r\n", timebuf);
    }
    if (complete > 0) 
    {
        printf("Connection: close\r\n");
        printf("\r\n");
    }
}

// Read the HTTP headers and translate them to 
// environment variables. Stop reading when we
// get a blank line so we know anything after
// that is form data.
void read_http_headers() {

    char buf[TOKEN_SIZE];
    char* name;
    char* value;
    char* env;
    int i;
    while (1) {

        // Read the next header, if it couldn't be read, mark
        // the stream as exhausted so we don't try to get
        // any more
        if (!fgets(buf, sizeof(buf), stdin)) {
            #ifdef LOGGING
                syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "Stream is exhausted, no more data");
            #endif
            exhausted = 1;
            break;
        }

        // Stop reading headers if we get a blank line
        if (*buf == '\r' || *buf == '\n' || *buf == '\0') {
            #ifdef LOGGING
                syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "Got blank line - end of HTTP headers (%s)", buf);
            #endif
            break;
        }

        #ifdef LOGGING
            syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "Got HTTP header: %s", buf);
        #endif

        // Split the line into name/value
        name = strtok(buf, ":");
        value = strtok(NULL, "\r");

        // Remove leading space
        if (*value == ' ') value++;

        // Swap any hyphens in the name for underscores and
        // upper case any letters
        for (i = 0; i < strlen(name); i++)
            if (*(name + i) == '-') 
                (*(name + i)) = '_';
            else
                (*(name + i)) = toupper(*(name + i));

        // Allocate the environment variable string dynamically
        // as each memory block pointed to by env becomes
        // part of the environment and we don't want to
        // overwrite vars (no, we don't need to free it)
        env = (char*) malloc(TOKEN_SIZE);
        snprintf(env, TOKEN_SIZE, "HTTP_%s=%s", name, value);
        putenv(env);

        #ifdef LOGGING
            syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "CGI env variable: %s", env);
        #endif
    }

}

void send_error(int status, char* title, char* extra, char* text)
{
    send_headers(status, title, extra, "text/html", -1, -1, 1);
    printf("<HTML><HEAD><TITLE>%d %s</TITLE></HEAD>\r\n", status, title);
    printf("<BODY><H4>%d %s</H4>\r\n", status, title);
    printf("%s\r\n", text);
    printf("</BODY></HTML>\r\n");

    #ifdef ONELOG
        logrequest(status, -1);
    #endif
}

#ifdef CGI
void handle_cgi(char* path, char* vpath, char* querystring, int ispost)
{
    // CGI output buffer
    char output[CGI_BUFFER];

    // Set the environment variables for the CGI (we already did
    // the HTTP headers earlier)
    char server_software[TOKEN_SIZE];
    snprintf(server_software, sizeof(server_software), "SERVER_SOFTWARE=%s", SERVER);

    char server_name[TOKEN_SIZE];
    char* hostname = (char*) getenv("HOSTNAME");
    if (!hostname) hostname = "";
    snprintf(server_name, sizeof(server_name), "SERVER_NAME=%s", hostname);

    char script_name[TOKEN_SIZE];
    char* qs = strstr(vpath, "?");
    if (qs) *(qs) = '\0';
    snprintf(script_name, sizeof(script_name), "SCRIPT_NAME=%s", vpath);

    char query_string[TOKEN_SIZE];
    snprintf(query_string, sizeof(query_string), "QUERY_STRING=%s", querystring);

    putenv(server_software);
    putenv(server_name);
    putenv(script_name);
    putenv(query_string);
    putenv("GATEWAY_INTERFACE=CGI/1.0");
    putenv("SERVER_PROTOCOL=HTTP/1.0");
    if (!ispost)
        putenv("REQUEST_METHOD=GET");
    else
        putenv("REQUEST_METHOD=POST");
    putenv("PATH_INFO=");
    putenv("PATH_TRANSLATED=");

    // Change the current working directory to the CGI 
    // directory before we start
    char dir[TOKEN_SIZE];
    strcpy(dir, path);
    int i;
    for (i = strlen(dir); i > 0; i--) {
        if ( *(dir + i) == '/' ) {
            *(dir + i) = '\0';
            break;
        }
    }
    #ifdef LOGGING
        syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "chdir to %s", dir);
    #endif
    chdir(dir);

    int bytesread = 0;
    int retval = 0;
    char buf[TOKEN_SIZE];

    // If we have a POST, read stdin and flush it to our
    // CGI script. We have to use bidirectional communication
    // for a post.
    if (ispost && !exhausted) {

        // Make stdin use non-blocking IO
        fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

        // Execute the CGI
        int infp, outfp;
        popen2(path, &infp, &outfp);

        while (1) {
            // Read the posted data
            #ifdef LOGGING
                syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "POST - reading form data from stdin");
            #endif
            bytesread = fread(buf, 1, sizeof(buf), stdin);
            if (bytesread <= 0) break;

            #ifdef LOGGING
                syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "Read %d bytes from stdin", bytesread);
            #endif

            // Send it to our CGI script
            write(infp, buf, bytesread);

            #ifdef LOGGING
                syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "Wrote %d bytes to cgi", bytesread);
            #endif
        }
        close(infp);

        // Wait 0.5s for the CGI's output buffer to fill
        // before we read it. This is a shit solution, but re-enabling
        // blocking for the pipe doesn't seem to help. Hmm.
        usleep(500000);

        // Read the CGI's output
        bytesread = read(outfp, output, sizeof(output));
        retval = close(outfp);

    }
    else {

        // We can use normal blocking IO for get requests,
        // making it a bit more stable

        #ifdef LOGGING
            syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "GET, no need to send form data");
        #endif

        // Execute the CGI and read the output
        FILE* p = popen(path, "r");
        bytesread = fread(output, 1, sizeof(output), p);
        retval = pclose(p);
    }

    // Split the CGI output into header
    // and body. We do this by looking for the double blank
    // line marker, then replacing it with a string
    // terminator and setting pointers to different points
    // in the output for header and body
    char* headers;
    char* body;
    char* marker;
    int markerlen = 4;
    marker = strstr(output, "\r\n\r\n");
    if (!marker) {
        marker = strstr(output, "\n\n");
        markerlen = 2;
    }
    if (!marker) {
        // No HTTP headers have been given in the CGI
        // output. Spec is unclear what to do here,
        // so we'll just send our default headers
        headers = "\0";
        body = output;
    }
    else {
        // Put a terminator in at the end of the headers
        // and set the body to after the marker. Adjust the
        // length to remove the headers and marker
        *marker = '\0';
        headers = output;
        body = marker + markerlen;
        bytesread -= strlen(headers) + markerlen;
    }

    // Response code
    int status = 200;

    // Is the CGI output a redirect? 
    int isredirect = 0;
    char redirect[TOKEN_SIZE];
    char* locstart = strstr(headers, "Location:");
    if (locstart) {
        isredirect = sscanf(locstart, "Location: %s", redirect);
        if (isredirect > 0) status = 302;
        #ifdef LOGGING
            syslog(LOG_MAKEPRI(LOG_DAEMON, LOG_DEBUG), "Redirect to location: %s", redirect);
        #endif
    }

    // Success error code, send the output
    if (retval == 0) 
    {

        send_headers( status, "OK", headers, NULL, bytesread, -1, 1 );
        fwrite(body, 1, bytesread, stdout);
        #ifdef ONELOG
            logrequest(status, bytesread);
        #endif
    }
    // An error must have occurred - send a 500
    else 
    {
        send_error( 500, "Internal Server Error", NULL, "An error occurred serving the document requested." ); 
        #ifdef ONELOG
            logrequest(500, -1);
        #endif
    }
}
#endif // CGI

void send_file(char* path, struct stat *statbuf, char* vpath, char* querystring, int ispost)
{
    char data[4096];
    int n;
    char* mime_type = get_mime_type(path);

    FILE *file = fopen(path, "r");
    if (!file) 
    {
        send_error(403, "Forbidden", NULL, "Access denied.");
        return;
    }

    #ifdef CGI
    // Handle as a CGI script if that's what it is
    if (strcmp(mime_type, "cgi") == 0) 
    {
        fclose(file);
        handle_cgi(path, vpath, querystring, ispost);
        return;
    }
    #endif

    // Grab the file length
    int length = S_ISREG(statbuf->st_mode) ? statbuf->st_size : -1;

    // Log the file request (CGI handles the log itself)
    #ifdef ONELOG
        logrequest(200, length);
    #endif

    // Send the file
    send_headers( 200, "OK", NULL, mime_type, length, statbuf->st_mtime, 1);

    while ((n = fread(data, 1, sizeof(data), file)) > 0) fwrite(data, 1, n, stdout);
    fclose(file);
}

/**
 * Unescape URL encoded stuff
 */
void unescape(char* str, char* output) {

    char* c = str;
    char* o = output;
    char* end = (str + strlen(str));
    while (c < end) {
        if (strncmp(c, "%20", 3) == 0) {
            *o = ' '; c++; c++; o++;
        }
        else if (strncmp(c, "%24", 3) == 0) {
            *o = '$'; c++; c++; o++;
        }
        else if (strncmp(c, "%26", 3) == 0) {
            *o = '&'; c++; c++; o++;
        }
        else if (strncmp(c, "%2B", 3) == 0) {
            *o = '+'; c++; c++; o++;
        }
        else if (strncmp(c, "%2C", 3) == 0) {
            *o = ','; c++; c++; o++;
        }
        else if (strncmp(c, "%2F", 3) == 0) {
            *o = '/'; c++; c++; o++;
        }
        else if (strncmp(c, "%3A", 3) == 0) {
            *o = ':'; c++; c++; o++;
        }
        else if (strncmp(c, "%3B", 3) == 0) {
            *o = ';'; c++; c++; o++;
        }
        else if (strncmp(c, "%3D", 3) == 0) {
            *o = '='; c++; c++; o++;
        }
        else if (strncmp(c, "%3F", 3) == 0) {
            *o = '?'; c++; c++; o++;
        }
        else if (strncmp(c, "%40", 3) == 0) {
            *o = '@'; c++; c++; o++;
        }
        else {
            *o = *c;
            o++;
        }
        c++;
    }
    *o = '\0';
}

int process(char* documentbase)
{
    char buf[TOKEN_SIZE];
    char* method;
    char* path;
    char* protocol;
    char* querystring;
    char pathbuf[TOKEN_SIZE];
    char thepath[TOKEN_SIZE];
    char realpath[TOKEN_SIZE];
    struct stat statbuf;
    int len, i;
    int ispost = 0;

    if (!fgets(buf, sizeof(buf), stdin)) return -1;

    // Copy the request before we muck about with it and
    // remove the eof
    strcpy(request, buf);
    for (i = strlen(request); i > 0; i--) {
        if ( *(request + i) == '\r' ) {
            *(request +i) = '\0';
            break;
        }
    }

    // Get the individual bits of the request
    method = strtok(buf, " ");
    path = strtok(NULL, " ");
    protocol = strtok(NULL, "\r");

    // Stop if the request is incomplete
    if (!method || !path || !protocol) return -1;

    // Parse querystring if there is one
    querystring = strstr(path, "?");
    if (querystring)
        querystring += sizeof(char);

    // Use the documentbase to find the full path to the document
    snprintf(thepath, sizeof(thepath), "%s%s", documentbase, path);

    // Unescape any URL encoding
    unescape(thepath, realpath);

    // Turn the querystring ? into a string terminator in the
    // file path since we aren't interested in it to find the file
    char* qsloc = strstr(realpath, "?");
    if (qsloc)
        *(qsloc) = '\0';

    // Read the HTTP headers and turn them into environment variables
    read_http_headers();

    // Flag posts so we know to redirect input when grabbing CGIs
    if (strcmp(method, "POST") == 0) ispost = 1;

    // We only support GET and POST requests
    if (strcmp(method, "GET") != 0 && strcmp(method, "POST") != 0)
        send_error(501, "Not supported", NULL, "Method is not supported.");

    #ifdef NORELATIVE
    // Send forbidden for relative paths if we're stripping
    else if (strstr(realpath, ".."))
        send_error(403, "Forbidden", NULL, "Permission denied.");
    #endif

    // Drop out if the path is invalid
    else if (stat(realpath, &statbuf) < 0)
        send_error(404, "Not Found", NULL, "File not found.");

    // Path given is a directory
    else if (S_ISDIR(statbuf.st_mode))
    {
        len = strlen(realpath);
        // If it's a directory, redirect with a slash on the end
        if (len == 0 || realpath[len - 1] != '/')
        {
            snprintf(pathbuf, sizeof(pathbuf), "Location: %s/", path);
            send_error(302, "Found", pathbuf, "Directories must end with a slash.");
        }
        else
        {
            // If the directory has an index page, send that
            snprintf(pathbuf, sizeof(pathbuf), "%s%sindex.html", documentbase, path);
            if (stat(pathbuf, &statbuf) >= 0)
                send_file(pathbuf, &statbuf, NULL, NULL, 0);

            // Otherwise, generate a listing page and send that
            else
            {
                DIR *dir;
                struct dirent *de;

                send_headers(200, "OK", NULL, "text/html", -1, statbuf.st_mtime, 1);
                printf("<HTML><HEAD><TITLE>Index of %s</TITLE></HEAD>\r\n<BODY>", path);
                printf("<H4>Index of %s</H4>\r\n<PRE>\n", path);
                printf("Name Last Modified Size\r\n");
                printf("<HR>\r\n");
                if (len > 1) printf("<A HREF=\"..\">..</A>\r\n");

                dir = opendir(realpath);
                while ((de = readdir(dir)) != NULL)
                {
                    char timebuf[32];
                    struct tm *tm;

                    strcpy(pathbuf, realpath);
                    strcat(pathbuf, de->d_name);

                    stat(pathbuf, &statbuf);
                    tm = gmtime(&statbuf.st_mtime);
                    strftime(timebuf, sizeof(timebuf), "%d-%b-%Y %H:%M:%S", tm);

                    printf("<A HREF=\"%s%s\">", de->d_name, S_ISDIR(statbuf.st_mode) ? "/" : "");
                    printf("%s%s", de->d_name, S_ISDIR(statbuf.st_mode) ? "/</A>" : "</A> ");
                    if (strlen(de->d_name) < 32) printf("%*s", 32 - strlen(de->d_name), "");

                    if (S_ISDIR(statbuf.st_mode))
                        printf("%s\r\n", timebuf);
                    else
                        printf("%s %10d\r\n", timebuf, statbuf.st_size);
                }
                closedir(dir);

                printf("</PRE>\r\n<HR>\r\n<ADDRESS>%s</ADDRESS>\r\n</BODY></HTML>\r\n", SERVER);
            }
        }
    }
    else
    {
        send_file(realpath, &statbuf, path, querystring, ispost);
    }

    return 0;
}

int main(int argc, char* argv[])
{
    char* docbase = "/var/www";
    if (argc > 1)
        docbase = argv[1];
    process(docbase);
    return 0;
}
