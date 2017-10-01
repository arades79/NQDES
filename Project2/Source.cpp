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
		key = key % 0xFFFFFFFF;
	}
		return key;
}


 int seededrandomiser(int seed)
{
	return abs(seed += (seed * seed) | 5);
}


string encrypt(string message, int startKey) 
{
	int keys[3];
	keys[0] = startKey;
	keys[1] = seededrandomiser(keys[0]);
	keys[2] = seededrandomiser(keys[1]);
	for (int encCount = 0; encCount <= 2; encCount++) {
		int key = keys[encCount];
		string keystring = to_string(key);
		size_t subkeyLength = keystring.length();
	//do the full algorithm 3 times
	
		//substitution element of algorithm
		for (int i = 0; i < message.length(); i++) {
			int subkey = (int)keystring[i % subkeyLength];
			int cipher = (int) message[i];
			cipher -= 33;	
			cipher += (key ^ subkey);
			cipher = cipher % 94;
			cipher += 33;
			message[i] = (char)cipher;
		}
		//transposition element of algorithm
		for (int j = 0; j < ((message.length()) / 2); j++) //execute this for loop for half of the length of the string 'message'
		{
			int subkey = (int)keystring[j % subkeyLength]; //creates a different value for 'subkey' for each element of the string 'message'
			if ((key ^ subkey) % 2 == 0) //determines whether or not the values of the two elements of the string 'message' being evaluated should be swapped
			{
				char temp = message[j]; //assign the value contained at the ith element of the string 'message' to a temporary variable named 'temp'
				message[j] = message[(message.length() - 1) - j]; //set the value contained at the ith element of the string 'message' equal to the value contained at 'message[(message.length()-1) - i]'
				message[(message.length() - 1) - j] = temp; //set the value contained at 'message[(message.length()-1) - i]' equal to the value contained within the variable 'temp'
			}
		}

	}

	return message;
}


string decrypt(string cipher, int startKey) 
{
	int keys[3];
	keys[2] = startKey;
	keys[1] = seededrandomiser(keys[2]);
	keys[0] = seededrandomiser(keys[1]);
	for (int decCount = 0; decCount <= 2; decCount++) {
		int key = keys[decCount];
		string keystring = to_string(key);
		size_t subkeyLength = keystring.length();
		//do the full algorithm 3 times


		for (int j = 0; j < ((cipher.length()) / 2); j++) //execute this for loop for half of the length of the string 'message'
		{
			int subkey = (int)keystring[j % subkeyLength]; //creates a different value for 'subkey' for each element of the string 'message'
			if ((key ^ subkey) % 2 == 0) //determines whether or not the values of the two elements of the string 'message' being evaluated should be swapped
			{
				char temp = cipher[j]; //assign the value contained at the ith element of the string 'message' to a temporary variable named 'temp'
				cipher[j] = cipher[(cipher.length() - 1) - j]; //set the value contained at the ith element of the string 'message' equal to the value contained at 'message[(message.length()-1) - i]'
				cipher[(cipher.length() - 1) - j] = temp; //set the value contained at 'message[(message.length()-1) - i]' equal to the value contained within the variable 'temp'
			}
		}

		//substitution element of algorithm
		for (int i = 0; i < cipher.length(); i++) {
			int subkey = (int)keystring[i % subkeyLength];
			int plaintext = (int)cipher[i];
			plaintext -= 33;
			plaintext -= (key ^ subkey);
			while (plaintext < 0) {
				plaintext += 94;
			}
			plaintext = plaintext % 94;
			plaintext += 33;
			cipher[i] = (char)plaintext;
		}
	}

	return cipher;
}



void main() 
{
	string message;

	char userInput;
	cout << "encrypting (e) or decrypting (d)?\n";
	cin >> userInput;

	string password;
	while (password.length() < 8) {
		cout << "\nput in your password (minimum 8 characters)\n";
		cin >> password;
	}
	int key = passwordHash(password);


	switch (userInput) {
	case 'e':
		cout << "put in your message (no spaces or non ascii characters) \n";
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