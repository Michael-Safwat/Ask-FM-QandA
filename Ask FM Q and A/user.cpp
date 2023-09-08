#include"user.h"

user::user()
{

}
user::user(const std::vector<std::string>& data)
{
	{
		user_id = stoi(data[0]);
		username = data[1];
		password = data[2];
		fullname = data[3];
		email = data[4];
		accepts_anonymous_questions = stoi(data[5]);
	}
}

const int user::get_user_id ()const
{
	return user_id;
}
void user::set_user_id(const int user_id)
{
	this->user_id = user_id;
}

const std::string& user::get_username()const 
{
	return username;
}
void user::set_username(const std::string& username)
{
	this->username = username;
}

const std::string& user::get_password()const
{
	return password;
}
void user::set_password(const std::string& password)
{
	this->password = password;
}

const std::string& user::get_email()const
{
	return email;
}
void user::set_email(const std::string& email)
{
	this->email = email;
}

const std::string& user::get_fullname()const
{
	return fullname;
}
void user::set_fullname(const std::string& fullname)
{
	this->fullname = fullname;
}

const bool user::get_accepts_anonymous_questions()const 
{
	return accepts_anonymous_questions;
}
void user::set_accepts_anonymous_questions(const int allow_anonymous_questions)
{
	this->accepts_anonymous_questions = allow_anonymous_questions;
}


bool user::is_registered(const std::string& username_, const std::string& password_)const
{
	if (get_username() == username_ && get_password() == password_)
		return true;
	return false;
}
bool user::user_exists(const std::string& username_, const std::string& email_)const
{
	if (get_username() == username_ || get_email() == email_)
		return true;
	return false;
}
const std::string user::To_String()
{
	std::ostringstream oss;
	oss << get_user_id() << "~" << get_username() << "~" << get_password() << "~" << get_fullname() 
		<< "~" << get_email() << "~" << get_accepts_anonymous_questions();

	return oss.str();
}

bool user:: login(const std::vector<user>& users)
{
	std::string username, password;

	std::cout << "Enter username & password: ";
	std::cin >> username >> password;

	for (auto user : users)
	{
		if (user.is_registered(username, password))
		{
			std::cout << "Welcome!\n\n";
			set_user_id(user.get_user_id());
			set_username(username);
			set_password(password);
			set_email(user.get_email());
			set_fullname(user.get_fullname());
			set_accepts_anonymous_questions(user.get_accepts_anonymous_questions());
			return true;
		}
	}
	std::cout << "There is no user with such credentials...\n\n";
	return false;
}
bool user::signup(const std::vector<user>& users, int last_user_id)
{
	std::string username, password, fullname, email;
	bool allow_anonymous_questions;

	std::cout << "Enter username (no spaces): ";
	std::cin >> username;
	std::cout << "Enter password: ";
	std::cin >> password;
	std::cout << "Enter fullname (no spaces): ";
	std::cin >> fullname;
	std::cout << "Enter E-mail: ";
	std::cin >> email;
	std::cout << "Allow anonymous questions?: ";
	std::cin >> allow_anonymous_questions;

	for (auto user : users)
	{
		if (user.user_exists(username, email))
		{
			std::cout << "Username or E-mail already in use...\n";
			return false;
		}
	}

	set_user_id(last_user_id);
	set_username(username);
	set_password(password);
	set_email(email);
	set_fullname(fullname);
	set_accepts_anonymous_questions(allow_anonymous_questions);

	return true;
}
void user::print_q_to_me(const std::vector<question>& questions)const
{
	for (auto question : questions)
	{
		if (question.get_receiver_id() == get_user_id())
		{
			if (question.get_parent_id() != -1)
			{
				std::cout << "\tThread: ";
			}
			std::cout << "Question ID (" << question.get_question_id() << ") ";
			if (!question.get_is_asked_anonymously())
			{
				std::cout << "from User ID (" << question.get_sender_id() << ") ";
			}
			std::cout << "\t\tQuestion: " << question.get_question_body() << "\n";
			if (question.get_answer_body() != "")
			{
				std::cout << "\tAnswer: " << question.get_answer_body() << "\n";
			}
			std::cout << "\n";
		}
	}
}
void user::print_q_from_me(const std::vector<question>& questions)const
{
	for (auto question : questions)
	{
		if (question.get_sender_id() == get_user_id())
		{
			std::cout << "Question ID (" << question.get_question_id() << ") ";
			if (!question.get_is_asked_anonymously())
			{
				std::cout << "!AQ ";
			}
			else
			{
				std::cout << "    ";
			}
			std::cout << "to User ID (" << question.get_receiver_id() << ") " << "\t\t";
			std::cout << "Question: " << question.get_question_body();
			if (question.get_answer_body() != "")
				std::cout << "\tAnswer: " << question.get_answer_body();
			else
			{
				std::cout << "\tNot Answered Yet";
			}
			std::cout << "\n\n";
		}
	}
}