#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;


int passwordHash(string password) 
{
	size_t length = password.length();
	int key = 0;
	for (size_t i = 0; i < length; i++){
		//we add the values of each character in the password to generate a sort-of hash
		key += (int) password[i];
	}
		return key;
}


 int seededrandomiser(int seed)
{//pseudo-random generator that will give the same results given the same seed, but sequential results appear random
	return abs(seed += (seed * seed) | 5);
}


string encrypt(string message, int startKey) 
{
	int keys[3]; //define array to store key for each iteration
	keys[0] = startKey;
	keys[1] = seededrandomiser(keys[0]);
	keys[2] = seededrandomiser(keys[1]);
	for (int encCount = 0; encCount <= 2; encCount++) {
		//each iteration of the loop use the next in the sequence from the PRNG to make results seem drasitcally different each step
		int key = keys[encCount];
		string keystring = to_string(key); //get the character representations of the digits in the key
		size_t subkeyLength = keystring.length(); //get the length to use as a mask to wrap the contents of the key
	//do the full algorithm 3 times
	
		//substitution element of algorithm
		for (int i = 0; i < message.length(); i++) {
			int subkey = (int) keystring[i % subkeyLength]; //use the value of the character representation of the digit to make each character of the message offset by a different value
			int cipher = (int) message[i];
			cipher -= 33; //subtract mask of ascii control characters to make sure none of them end up in the ciphertext
			cipher += (key ^ subkey); // offset the character by the full key, and the subkey that will change for each character
			cipher = cipher % 94; // keep character within easily printable ascii range
			cipher += 33; // re-add the mask to keep characters out of ascii control codes
			message[i] = (char)cipher; //replace the character of the message with our newly offset character
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
	//we place the keys in the inverse of their array position as in encryption
	keys[2] = startKey;
	keys[1] = seededrandomiser(keys[2]);
	keys[0] = seededrandomiser(keys[1]);
	for (int decCount = 0; decCount <= 2; decCount++) {
		int key = keys[decCount];
		string keystring = to_string(key);
		size_t subkeyLength = keystring.length();
		
		//to decrypt we must un-transpose, then un-substitute (reverse order of encryption)

		//transposition element of algorithm
		for (int j = 0; j < ((cipher.length()) / 2); j++) 
		{//transposition algorithm reverses itself with an equivalent key
			int subkey = (int)keystring[j % subkeyLength]; 
			if ((key ^ subkey) % 2 == 0) 
			{
				char temp = cipher[j]; 
				cipher[j] = cipher[(cipher.length() - 1) - j];
				cipher[(cipher.length() - 1) - j] = temp;
			}
		}

		//substitution element of algorithm
		for (int i = 0; i < cipher.length(); i++) {
			//for inverse substitution, we must subtract our key values from the cipher, then add multiples of our usable ascii range until we're in the valid character range
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

	//prompt the user on if they are encrypting or decrypting
	char userInput;
	cout << "encrypting (e) or decrypting (d)?\n";
	cin >> userInput;

	//prompt the user for their password
	string password;
	while (password.length() < 8) {
		cout << "\nput in your password (minimum 8 characters)\n";
		cin >> password;
	}
	int key = passwordHash(password);


	switch (userInput) {

	case 'e':
		//user wants to encrypt, and must imput a message to encrypt
		cout << "put in your message (no spaces or non ascii characters) \n";
		cin >> message;
		message = encrypt(message, key);
		cout << message;
		break;

	case 'd':
		//user wants to decrypt, and must input a previously encrypted ciphertext
		cout << "put your ciphertext\n";
		cin >> message;
		message = decrypt(message, key);
		cout << message;
		break;

	default:
		return;

	}	

	//basically a hack to pause the program until the user inputs something
	char shit;
	cin >> shit;

	return;
}