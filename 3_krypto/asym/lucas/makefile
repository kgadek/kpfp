all: key sign verify
key: key.c
	gcc key.c -o key -lssl -lcrypto
sign: sign.c
	gcc sign.c -o sign -lssl -lcrypto
verify: verify.c
	gcc verify.c -o verify -lssl -lcrypto