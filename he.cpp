// Copyright Nedelcu Horia, Nichita Radu

#include <bits/stdc++.h>
using namespace std;

#define MAX_LEN_PASSWORD 25
#define MIN_LEN_PASSWORD 10
#define MAX_SUFFIX_PREFIX_LEN 5

// Baza de date cu cuvinte random
// pentru generare de parole fake:
vector<string> common_words = {"dog",
 "car","ball", "1234", "243", "23", "64"};


// Baza de date parole legite:
map<string, string> legit_password_to_message;

// Baza de date parole fake:
map <string, string> fake_password_to_message;

void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

inline int new_fake_password_len() {
    return MIN_LEN_PASSWORD
        + rand() % MIN_LEN_PASSWORD;
}

void create_new_fake_passwords(string password, string fake_msg) {
    static char buffer_rand[MAX_LEN_PASSWORD];
    static const int fake_num = 2;
    string fake_passwd; 

    // passwords for brute-force attacks
    gen_random(buffer_rand, new_fake_password_len());
    fake_password_to_message.insert(make_pair(string(buffer_rand), fake_msg));

    // Suffix fake passwords:
    for (int i = 0; i < fake_num; ++i) {
        gen_random(buffer_rand, rand() % MAX_SUFFIX_PREFIX_LEN);
        fake_passwd = password + string(buffer_rand);

        if (legit_password_to_message.find(fake_passwd) ==
                legit_password_to_message.end()) {
            fake_password_to_message.insert(make_pair(fake_passwd, fake_msg));
        }
    }

    // Prefix fake passwords:
    for (int i = 0; i < fake_num; ++i) {
        gen_random(buffer_rand, rand() % MAX_SUFFIX_PREFIX_LEN);
        fake_passwd = string(buffer_rand) + password;

        if (legit_password_to_message.find(fake_passwd) ==
                legit_password_to_message.end()) {
            fake_password_to_message.insert(make_pair(fake_passwd, fake_msg));
        }
    }
    
    // Common words fake passwords:
    for (auto word : common_words) {
        fake_passwd = word + password;

        if (legit_password_to_message.find(fake_passwd) ==
                legit_password_to_message.end()) {
            fake_password_to_message.insert(make_pair(fake_passwd, fake_msg));
        }

        fake_passwd = password + word;

        if (legit_password_to_message.find(fake_passwd) ==
                legit_password_to_message.end()) {
            fake_password_to_message.insert(make_pair(fake_passwd, fake_msg));
        }
    }
}

void insert_corect_password(std::string password, std::string message) {

    legit_password_to_message[password] = message;
    if (fake_password_to_message.find(password) != fake_password_to_message.end()){
        fake_password_to_message.erase(password);
    }
    create_new_fake_passwords(password, "fake message");
}

bool check_if_password_was_guessed(std::string password) {
    if (legit_password_to_message.find(password) != legit_password_to_message.end()) {
        cout<<"Parola a fost corecta, iar mesajul continut de aceasta este:" << legit_password_to_message[password] << "\n";

    } else if (fake_password_to_message.find(password) != fake_password_to_message.end()) {
        cout<<"Mesaj = "<<fake_password_to_message[password]<<"\n";
        ofstream bad_guesses;
        bad_guesses.open("logout_bad_passwords.txt", ios_base::app);
        bad_guesses << "S-a introdus o parola gresita de la utilizator malitios" << password<<"\n";
        bad_guesses.close();
    } else {
        std::cout<<"Parola nu a fost gasita. Va rugam sa reincercati\n";
        return false;
    }

    return true;

}

int main() {

    // TODO: comezi de inregistrare
        // user introduce password, iar pe baza parola -> baza legit
        // cream random duse in fake database (nu exista in legit)

    // TODO: comenzi de login
        // verificam unde gasim parola
        // (pe baza baze de date stim dace e legit sau atacator)

        cout<<"Salut. Alege ce vrei sa faci?\n";
        cout<<"0 - Login\n";
        cout<<"1 - Try\n";
        cout<<"2 - Exit\n";

        string user_answer;

        while (1) {
            getline(cin, user_answer);
            if (user_answer.compare("Login") == 0) {
                std::cout<<"Va rugam sa introduceti o parola\n";
                std::string password;
                getline(cin, password);
                std::cout<<"Va rugam sa introduceti un mesaj\n";
                std::string message;
                getline(cin, message);
                insert_corect_password(password, message);
                std::cout<<"Datele au fost introduse cu success\n";
            } else if (user_answer.compare("Try") == 0) {
                std::cout<<"Va rugam sa introduceti parola dvs\n";
                std::string password;
                getline(cin, password);
                while (check_if_password_was_guessed(password) == false) {
                    std::cout<<"Va rugam sa introduceti in nou\n";
                    getline(cin, password);
                }
            } else if (user_answer.compare("Exit") == 0) {
                cout<<"Multumim ca ati incercat!\n";
                break;
            } else {
                cout<<"Comanda invalida. Comenzile valide sunt:\n";
                cout<<"0 - Login\n";
                cout<<"1 - Try\n";
                cout<<"2 - Exit\n";
            }
        }
}
