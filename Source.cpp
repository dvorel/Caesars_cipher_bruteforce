#include<thread>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
#include<iostream>
#include<clocale>
#include<thread>


#include<fcntl.h>
#include<io.h>
#include<codecvt>
#include <sstream>
#include <fstream>


using namespace std;
#pragma warning(disable:4996)


const char* dictDir = "C:\\caesar\\hr.txt";
const char* encDir = "C:\\caesar\\rtgde_zp_lab.txt";

int searchStr(wchar_t c, wstring key) {
	for (int i = 0; i < key.size(); i++) {
		if (c == key[i])
			return i; 
	}
	return -1;
}

bool searchVec(wstring w, vector<wstring>* key) {
	if (find(key->begin(), key->end(), w) != key->end()) { return true; }
	return false;
}


bool wordsKey(wstring enc, wstring dec, wstring* keyEnc, wstring* keyDec) {
	if (enc.length() != dec.length())
		return false;
	int oKey;
	int iKey;

	for (int i = 0; i < enc.length(); i++) {
		//find matches in generated key

		iKey = searchStr(enc[i], (*keyEnc));
		oKey = searchStr(dec[i], (*keyDec));

		if (iKey == -1 && oKey == -1) {
			keyEnc->push_back(enc[i]);
			keyDec->push_back(dec[i]);
		}
		else if (iKey != -1) {
			if ((*keyEnc)[iKey] != enc[i] || (*keyDec)[iKey] != dec[i]) {
				return false;
			}
		}
		else if (oKey != -1) {
			if ((*keyEnc)[oKey] != enc[i] || (*keyDec)[oKey] != dec[i]) {
				return false;
			}

		}
	}
	return true;
}

wstring encrypt(wstring in, wstring* keyEnc, wstring* keyDec) {
	wstring s = L"";
	int iKey;

	for (int i = 0; i < in.length(); i++) {
		iKey = searchStr(in[i], *keyDec);
		if (iKey == -1) {
			return L"";
		}
		s += keyEnc[iKey];
	}
	return s;
}

wstring decrypt(wstring in, wstring* keyEnc, wstring* keyDec){
	wstring s = L"";
	int oKey;

	for (int i = 0; i < in.length(); i++) {
		oKey = searchStr(in[i], *keyEnc);
		if (oKey == -1) {
			return L"";
		}
		s += (*keyDec)[oKey];
	}
	return s;
}

vector<wstring> sameLen(wstring w, vector<wstring>* d) {
	vector<wstring> l;

	for (int i = 0; i < (*d).size(); i++) {
		if (w.length() == (*d)[i].length()) {
			l.push_back((*d)[i]);
		}
	}
	return l;
}

void writeFile(int n, wstring enc, vector<wstring> dec){
	string filenames = "C:\\caesar\\out-";
	filenames += to_string(n);
	const char* filename = filenames.c_str();

	wofstream wof(filename);
	wof.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	
	wof << enc << endl;
	wof << "----------" << endl;
	for (auto d : dec) {
		wof << d << endl;
	}
	wof.close();
}

std::vector<wstring> readFile(const char* filename, bool all)
{
	vector<wstring> l;
	wstring line;
	wstring word;

	wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	wstringstream wss;
	wss << wif.rdbuf();
	
	
	wchar_t tab = L'\t';
	wchar_t space = L' ';
	wchar_t nl = L'\n';
	wchar_t end = L'\0';

	while(getline(wss, line)) {
		for (int i = 0; i < line.length(); i++) {
			if (line[i] == tab || line[i] == space || line[i] == nl) {
				if (!all) {
					//word.push_back(tolower(line[i]));
					l.push_back(word);
					word.clear();
					break;
				}
				else {
					//word.push_back(tolower(line[i]));
					if (!searchVec(word, &l)) {
						l.push_back(word);
					}
					word.clear();
				}


			}
			else {
				word.push_back(tolower(line[i]));
			}
		}
		if (word.size() > 0 && all) {
			if (!searchVec(word, &l)) {
				l.push_back(word);
			}
			word.clear();
		}
	}
	return l;
}

void magic(wstring w1, wstring d1, wstring* wordNew, vector<wstring>* dictNew, wstring w2, wstring d2, vector<wstring>* enc, vector<wstring>* dict) {
	wstring newDec = d1;
	wstring newEnc = w1;
	bool contains = true;

	//if (!wordsKey(w1, d1, newEnc, newDec)) { return; }
	if (!wordsKey(w2, d2, &newEnc, &newDec)) { return; }

	for (int iWord = 0; iWord < (*enc).size(); iWord++) {
		contains = true;
		for (int i = 0; i < ((*enc)[iWord]).length(); i++) {
			if (searchStr(((*enc)[iWord])[i], newEnc) == -1) {
				contains = false;
				break;
			}
		}
		if (!contains)
			continue;

		wstring word;
		word = decrypt((*enc)[iWord], &newEnc, &newDec);

		if (!searchVec(word, dict)) { return; }
		else { break; }
	}

	if (contains) {
		if ((*wordNew) != newEnc) {
			(*wordNew) = newEnc;
		}
		(*dictNew).push_back(newDec);
	}
}

void threadMagic(wstring encrK, vector<wstring> decrK, wstring* encrNew, vector<wstring>* decrNew, wstring word, vector<wstring> wordDec, vector<wstring>* text, vector<wstring>* dict, unsigned int* counter, unsigned int max, int n) {
	double perc;
	for (int iKey = 0; iKey < decrK.size(); iKey++) {
		perc = (double(*counter) / double(max))*100;
		wcout << max << " | " << perc << endl;

		for (int iDict = 0; iDict < wordDec.size(); iDict++) {
			magic(encrK, decrK[iKey], encrNew, decrNew, word, wordDec[iDict], text, dict);
			(*counter)++;
		}
	}
}

vector<vector<wstring>> split(vector<wstring> vect, int n) {
	vector<vector<wstring>> l;
	int s = vect.size() / n;
	int ending = 0;

	for (int i = 0; i < n-1 ; i++) {
		auto first = vect.begin() + ending;
		auto last = vect.begin() + ending + s;
		ending += s;
		vector<wstring> temp(first, last);
		l.push_back(temp);
	}
	
	auto first = vect.begin() + ending;
	auto last = vect.end();
	vector<wstring> temp(first, last);
	l.push_back(temp);

	return l;
}



int main(void) {
	wcout << _setmode(_fileno(stdout), _O_U8TEXT) << endl;
	wstring encryptedK = L" ";	//encrypted
	vector<wstring> decryptedK;	//list of keys
	decryptedK.push_back(L" ");

	vector<wstring> encr, dict;
	encr = readFile(encDir, true);
	dict = readFile(dictDir, false);

	wcout << "Files loaded" << endl;
	wcout << "Encr words: " << encr.size() << endl;
	wcout << "Dict words: " << dict.size() << endl;

	unsigned int counter, max;
 	int n = 0;
	while (n < encr.size()) {
		wcout << "ITER: " << n << endl;
		wstring word = encr[n];
		n++;

		vector<wstring> decr = sameLen(word, &dict);		
		vector<vector<wstring>> decNew;
		vector<wstring> encNew;

		counter = 1;
		max = decryptedK.size() * decr.size();
		wcout << max << endl;
		if (decr.size() > 16) {
			vector<vector<wstring>> splitted = split(decr, 16);
			vector<thread> thr;
			

			for (int i = 0; i < splitted.size(); i++) {
				vector<wstring> s;
				decNew.push_back(s);
				wstring enc = L"";
				encNew.push_back(enc);

				/*
				//threadMagic(wstring encrK, vector<wstring> decrK, wstring* encrNew, vector<wstring>* decrNew, wstring word, vector<wstring> wordDec, vector<wstring>* text, vector<wstring>* dict, int* counter, int max, int n)
				thread t(threadMagic, encryptedK, decryptedK, &(encNew[_i]), decNew[_i], word, splitted[_i], &encr, &dict, &counter, max, _i);
				//t.join();
				thr.push_back(move(t));
				*/
			}

			
			thread t1(threadMagic, encryptedK, decryptedK, &(encNew[0]), &(decNew[0]), word, splitted[0], &encr, &dict, &counter, max, 0);
			thread t2(threadMagic, encryptedK, decryptedK, &(encNew[1]), &(decNew[1]), word, splitted[1], &encr, &dict, &counter, max, 1);
			thread t3(threadMagic, encryptedK, decryptedK, &(encNew[2]), &(decNew[2]), word, splitted[2], &encr, &dict, &counter, max, 2);
			thread t4(threadMagic, encryptedK, decryptedK, &(encNew[3]), &(decNew[3]), word, splitted[3], &encr, &dict, &counter, max, 3);
			thread t5(threadMagic, encryptedK, decryptedK, &(encNew[4]), &(decNew[4]), word, splitted[4], &encr, &dict, &counter, max, 4);
			thread t6(threadMagic, encryptedK, decryptedK, &(encNew[5]), &(decNew[5]), word, splitted[5], &encr, &dict, &counter, max, 5);
			thread t7(threadMagic, encryptedK, decryptedK, &(encNew[6]), &(decNew[6]), word, splitted[6], &encr, &dict, &counter, max, 6);
			thread t8(threadMagic, encryptedK, decryptedK, &(encNew[7]), &(decNew[7]), word, splitted[7], &encr, &dict, &counter, max, 7);
			thread t9(threadMagic, encryptedK, decryptedK, &(encNew[8]), &(decNew[8]), word, splitted[8], &encr, &dict, &counter, max, 8);
			thread t10(threadMagic, encryptedK, decryptedK, &(encNew[9]), &(decNew[9]), word, splitted[9], &encr, &dict, &counter, max, 9);
			thread t11(threadMagic, encryptedK, decryptedK, &(encNew[10]), &(decNew[10]), word, splitted[10], &encr, &dict, &counter, max, 10);
			thread t12(threadMagic, encryptedK, decryptedK, &(encNew[11]), &(decNew[11]), word, splitted[11], &encr, &dict, &counter, max, 11);
			thread t13(threadMagic, encryptedK, decryptedK, &(encNew[12]), &(decNew[12]), word, splitted[12], &encr, &dict, &counter, max, 12);
			thread t14(threadMagic, encryptedK, decryptedK, &(encNew[13]), &(decNew[13]), word, splitted[13], &encr, &dict, &counter, max, 13);
			thread t15(threadMagic, encryptedK, decryptedK, &(encNew[14]), &(decNew[14]), word, splitted[14], &encr, &dict, &counter, max, 14);
			thread t16(threadMagic, encryptedK, decryptedK, &(encNew[15]), &(decNew[15]), word, splitted[15], &encr, &dict, &counter, max, 15);

			t1.join();
			t2.join();
			t3.join();
			t4.join();
			t5.join();
			t6.join();
			t7.join();
			t8.join();
			t9.join();
			t10.join();
			t11.join();
			t12.join();
			t13.join();
			t14.join();
			t15.join();
			t16.join();
			

			/*
			for (auto& t : thr) {
				wcout << int(&t) << endl;
				if (t.joinable()) {
					t.join();
				}
			}
			*/

			vector<wstring> app;
			for (int de = 0; de < decNew.size(); de++) {
				for (int dd = 0; dd < (decNew[de]).size(); dd++) {
					app.push_back((decNew[de])[dd]);
				}
			}
			decryptedK = app;

			for (int de = 0; de < encNew.size(); de++) {
				if (encNew[de].size() > encryptedK.size()) {
					encryptedK = encNew[de];
					break;
				}
			}
			
		}
		else {
			vector<wstring> app;
			wstring encc;
			for (int iKey = 0; iKey < decryptedK.size(); iKey++) {
				for (int iDict = 0; iDict < decr.size(); iDict++) {
					wcout << "|E: " << encryptedK << " |D: " << decryptedK[iKey] << " |W: " << word << " |NW: " << decr[iDict] << " |N: " << iDict << endl;
					magic(encryptedK, decryptedK[iKey], &encc, &app, word, decr[iDict], &encr, &dict);
				}
			}
			decryptedK = app;
			encryptedK = encc;
		}

		writeFile(n, encryptedK, decryptedK);
		wcout << "File done!" << endl;
		wcout << encryptedK << endl;
		wcout << decryptedK[0] << endl;
		wcout << endl;
		wcout << "N: " << n << endl;


	}
	
	return 0;
}