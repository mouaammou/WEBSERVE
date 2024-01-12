import cgi
# while True:
# 	pass
a=open('test.txt','wb')

while True:
    try:
        st = input() + '\n'
    except EOFError:
        break
    a.write(st.encode())


a.close()




cgi.test()