#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string DATA_FILE = "users_data.dat";


unsigned long simpleHash(const string &password)
{
    unsigned long hash = 5381;

    for (char letter : password)
    {
        hash = (hash * 33) + letter;
    }

    return hash;
}

bool isValidEmail(const string &email)
{
    const regex pattern(R"((\w+)(\.{0,1}(\w+))*@(\w+)(\.(\w+))+)");
    return regex_match(email, pattern);
}

bool checkPasswordLength(const string &password)
{
    if (password.length() < 8 || password.length() > 16)
    {
        cout << "The password must be between 8 and 16 characters.\n";
        return false;
    }
    return true;
}

bool checkUserExists(const string &email)
{
    ifstream input(DATA_FILE);
    string storedEmail;
    unsigned long storedHash;

    while (input >> storedEmail >> storedHash)
    {
        if (storedEmail == email)
        {
            return true;
        }
    }
    return false;
}

bool passwordMatches(const string &email, const string &password)
{
    ifstream input(DATA_FILE);
    string storedEmail;
    unsigned long storedHash;

    while (input >> storedEmail >> storedHash)
    {
        if (storedEmail == email)
        {
            return storedHash == simpleHash(password);
        }
    }
    return false;
}

void signup()
{
    string email, password, confirmPassword;
    cout << "Welcome to sign up page.\n";

    while (true)
    {
        cout << "Enter email: ";
        cin >> email;

        if (!isValidEmail(email))
        {
            cout << "Please enter a valid email address.\n";
        }
        else if (checkUserExists(email))
        {
            cout << "Email already exists. Please login.\n";
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        cout << "Enter password: ";
        cin >> password;

        if (!checkPasswordLength(password))
        {
            continue;
        }

        cout << "Confirm password: ";
        cin >> confirmPassword;

        if (password == confirmPassword)
        {
            break;
        }
        cout << "Passwords do not match. Try again.\n";
    }

    ofstream output(DATA_FILE, ios::app);
    output << email << " " << simpleHash(password) << endl;
    cout << "Your account has been created successfully!\n";
}

void login()
{
    string email, password;
    cout << "Welcome to login page.\n";
    cout << "Enter email: ";
    cin >> email;
    cout << "Enter password: ";
    cin >> password;

    if (!checkUserExists(email))
    {
        cout << "Email not found. Please register first.\n";
    }
    else if (passwordMatches(email, password))
    {
        cout << "Login successful!\n";
    }
    else
    {
        cout << "Invalid password. Please try again.\n";
    }
}

void resetPassword()
{
    string email, oldPassword, newPassword, confirmPassword;
    cout << "Password Reset\n";
    cout << "Enter your email: ";
    cin >> email;

    if (!checkUserExists(email))
    {
        cout << "Email not found. Please register first.\n";
        return;
    }

    cout << "Enter current password: ";
    cin >> oldPassword;
    if (!passwordMatches(email, oldPassword))
    {
        cout << "Current password is incorrect.\n";
        return;
    }

    while (true)
    {
        cout << "Enter new password: ";
        cin >> newPassword;

        if (!checkPasswordLength(newPassword))
        {
            continue;
        }

        cout << "Confirm new password: ";
        cin >> confirmPassword;
        if (newPassword == confirmPassword)
        {
            break;
        }
        cout << "Passwords do not match. Try again.\n";
    }

    ifstream input(DATA_FILE);
    vector<string> users;
    string storedEmail;
    unsigned long storedHash;

    while (input >> storedEmail >> storedHash)
    {
        if (storedEmail == email)
        {
            users.push_back(email + " " + to_string(simpleHash(newPassword)));
        }
        else
        {
            users.push_back(storedEmail + " " + to_string(storedHash));
        }
    }
    input.close();

    ofstream output(DATA_FILE);
    for (const string &user : users)
    {
        output << user << endl;
    }

    cout << "Password reset successfully!\n";
}

int main()
{
    cout << "---------------------------------------------\n";
    cout << "|      Login and Registration System         |\n";
    cout << "---------------------------------------------\n";

    while (true)
    {
        int option;
        cout << "1. Register\n2. Login\n3. Reset Password\n4. Exit\n";
        cout << "Select a valid option: ";
        cin >> option;

        switch (option)
        {
        case 1: signup(); break;
        case 2: login(); break;
        case 3: resetPassword(); break;
        case 4: cout << "Thank you...!\n"; return 0;
        default: cout << "Please provide a valid selection.\n";
        }
    }
}
