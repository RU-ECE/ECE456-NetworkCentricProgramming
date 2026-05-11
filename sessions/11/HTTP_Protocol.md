# HTTP protocol

The web server has to print a header indicating the protocol

```
HTTP/1.1 200 OK
Content-Type: text/html

```

The MIME type is the type of the content.

| Extension      | MIME Type    |
| -------------- | ------------ |
| `.txt`         | `text/plain` |
| `.html`/`.htm` | `text/html`  |
| `.webp`        | `image/webp` |
| `.jpg`         | `image/jpeg` |
| `.png`         | `image/png`  |

you may assume anything you don't know is text/plain

So, for a .webp image (note, the third line MUST BE blank!)

```
HTTP/1.1 200 OK
Content-Type: image/webp

```

"Content-Type: image/webp\n\n"

after that write the BINARY FILE (everything in the .webp file) to the socket and close

.css text/css .js text/javascript ?? not sure, the AI sure thinks this exists? .js application/javascript

When you send a request to a server: http://localhost:8080/index.html

It goes to the named server at port 8000. If you don't say, the default port is 80.

http://localhost/index.html below 1024 is the kernel, so in order to write a program that opens teh "special" ports <
1024 you have to be root/admin

In order to get a file: http://localhost:8000/index.html
