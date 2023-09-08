#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include"question.h"

class user
{
private:
	std::string username;
	std::string password;
	std::string fullname;
	std::string email;
	int user_id;
	bool accepts_anonymous_questions;
	
public:

	user();
	user(const std::vector<std::string>& data);

	const int get_user_id()const;
	void set_user_id(const int user_id);

	const std::string& get_username()const;
	void set_username(const std::string& username);

	const std::string& get_password()const;
	void set_password(const std::string& password);

	const std::string& get_email()const;
	void set_email(const std::string& email);

	const std::string& get_fullname()const;
	void set_fullname(const std::string& fullname);

	const bool get_accepts_anonymous_questions()const;
	void set_accepts_anonymous_questions(const int allow_anonymous_questions);

	bool is_registered(const std::string& username_, const std::string& password_)const;
	bool user_exists(const std::string& username_, const std::string& email)const;
	const std::string To_String();
	
	bool login(const std::vector<user>& users);
	bool signup(const std::vector<user>& users,int last_user_id);
	void print_q_to_me(const std::vector<question>& questions)const;
	void print_q_from_me(const std::vector<question>& questions)const;
};
