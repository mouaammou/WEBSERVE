#generete auto response http

response = ""

response += "HTTP/1.1 200 OK\n"
response += "Content-Type: text/html\n"
response += "\n"

response += "<!DOCTYPE html>\n"
response += "<html>\n"
response += "<head>\n"
response += "<title>My Response</title>\n"
response += "</head>\n"

response += "<body>\n"
response += "<h1>Welcome to My Response</h1>\n"
response += "<p>This is a sample response with 20 lines.</p>\n"
response += "<ul>\n"
response += "<li>Line 1</li>\n"
response += "<li>Line 2</li>\n"
response += "<li>Line 3</li>\n"
response += "<li>Line 4</li>\n"
response += "<li>Line 5</li>\n"
response += "<li>Line 6</li>\n"
response += "<li>Line 7</li>\n"
response += "<li>Line 8</li>\n"
response += "<li>Line 9</li>\n"
response += "<li>Line 10</li>\n"
response += "</ul>\n"
response += "</body>\n"

response += "</html>\n"

print(response)

