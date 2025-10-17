# Testing-Encryption-Decryption
2° Homework of Cybersecurity in La Sapienza University of Rome 

**HOMEWORK** (Due Sunday, October 19, 2025, at 11:59):
1) Use the OpenSSL API to implement and test multiple encryption and decryption operations with a fixed 128-bit symmetric key, randomly generated at initialization, using CBC mode.
2) Focus on the following algorithms: AES, Camellia (used in China as a standard), and SM4
3) Compare **encryption** and **decryption** performance using three fixed input files: a 16-byte (the size of a block, you can notice the padding) text file, a 20KB text file, and a large (>2MB) binary file
4) Measure the execution times and present the result graphically (not tables, but drawings). Also, analyse and compare file sizes
5) include any source code in the report.
we can use whatever programming language, is suggested C/C++. 
Considering that report considers 3 files and for each file we have to do an encryption and decryption, for each 3 algorithm. 