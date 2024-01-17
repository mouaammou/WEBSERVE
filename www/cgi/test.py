import cgi
import cgitb
cgitb.enable()

print(f"Content-Type: text/html\n")
print("Content-Type: text/html\n")
print("<form method='post' action='/'>")
print("  Username: <input type='text' name='username'>")
print("  <input type='submit' value='Login'>")
print("</form>")
