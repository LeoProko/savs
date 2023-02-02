openssl req -new -x509 -days 30 -keyout server.key -out server.pem

cp server.key server.key.orig
openssl rsa -in server.key.orig -out server.key
rm server.key.orig
