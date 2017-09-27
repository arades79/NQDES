#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

int passwordHash(string password) 
{
	size_t length = password.length();
	int key = 0;
	for (size_t i = 0; i < length; i++){
		key += (int) password[i];
	}
		return key;
}

string encrypt(string message, int key) 
{
	string keystring = to_string(key);
	size_t subkeyLength = keystring.length();
	//do the algorithm 3 times
	for (int i = 0; i < 3; i++) {
		//substitution element of algorithm
		for (int i = 0; i < message.length(); i++) {
			int subkey = (int)keystring[i % subkeyLength];
			int cipher = (int) message[i];
			cipher -= 32;	
			cipher += (key ^ subkey);
			cipher = cipher % 96;
			cipher += 32;
			message[i] = (char)cipher;
		}
	}
	return message;
}

string decrypt(string cipher, int key) 
{
	string keystring = to_string(key);
	size_t subkeyLength = keystring.length();
	//do the algorithm 3 times
	for (int i = 0; i < 3; i++) {
		//substitution element of algorithm
		for (int i = 0; i < cipher.length(); i++) {
			int subkey = (int)keystring[i % subkeyLength];
			int plaintext = (int)cipher[i];
			plaintext -= 32;
			plaintext -= (key ^ subkey);
			while (plaintext < 0) {
				plaintext += 96;
			}
			plaintext = plaintext % 96;
			plaintext += 32;
			cipher[i] = (char)plaintext;
		}
	}
	return cipher;
}

void main() 
{
	string message;

	char userInput;
	cout << "encrypting (e) or decrypting (d)?";
	cin >> userInput;

	string password;
	while (password.length() < 8) {
		cout << "\nput in your password\n";
		cin >> password;
	}
	int key = passwordHash(password);


	switch (userInput) {
	case 'e':
		cout << "put in your message \n";
		cin >> message;
		message = encrypt(message, key);

		cout << message;
		break;
	case 'd':
		cout << "put your ciphertext\n";
		cin >> message;
		message = decrypt(message, key);
		cout << message;
		break;
	default:
		return;
	}

		
	


	

	char shit;
	cin >> shit;

	return;
}