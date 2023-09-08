#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include"user.h"
#include"question.h"

using namespace std;

/*************************************Global Functions**************************************************/

void save_to_database(const vector<string>& data, const string& file_path, bool append = true)
{
	auto status = ios::app;

	if (!append)
		status = ios::trunc;

	ofstream of(file_path.c_str(), status);

	for (auto d : data)
	{
		of << d << "\n";

	}
	of.close();
}

const vector<string> load_database(const string& file_path)
{
	ifstream data(file_path.c_str());
	string line;
	vector<string> lines;

	while (getline(data, line))
	{
		lines.push_back(line);
	}
	data.close();
	return lines;
}

const vector<string> data_collector(string& line)
{
	int pos = -1;
	vector<string> data;

	do
	{
		pos = line.find("~");
		data.push_back(line.substr(0, pos));
		line = line.substr(pos + 1);
	} while (pos != -1);

	return data;
}

/*************************************Global Functions**************************************************/

class users_manager
{
private:
	vector<user> users;
	int last_user_id;

public:

	users_manager()
	{
		load_users_database();

		if (users.empty())
			last_user_id = 100;
		else
			last_user_id = users.back().get_user_id();
	}

	const vector<user>& get_users()const
	{
		return users;
	}
	int& get_last_user_id()
	{
		return last_user_id;
	}

	void load_users_database()
	{
		users.clear();
		const vector<string>lines = load_database("users.txt");

		for (auto line : lines)
		{
			const vector<string> data = data_collector(line);
			user user(data);
			users.push_back(user);
		}
	}

	void list_system_users()
	{
		load_users_database();

		for (auto user : users)
		{
			cout << "ID: " << user.get_user_id() << "\t\t" << "Name: " << user.get_fullname() << "\n";
		}
		cout << "\n";
	}

};
class questions_manager
{
private:
	vector<question> questions;
	int last_question_id;

public:

	questions_manager()
	{
		load_questions_database();

		if (questions.empty())
			last_question_id = 100;
		else
			last_question_id = questions.back().get_question_id();
	}

	int& get_last_question_id()
	{
		return last_question_id;
	}
	vector<question>& get_questions()
	{
		return questions;
	}

	void load_questions_database()
	{
		questions.clear();
		const vector<string>lines = load_database("questions.txt");

		for (auto line : lines)
		{
			const vector<string> data = data_collector(line);
			question question(data);
			questions.push_back(question);
		}
	}
	void question_save_to_database(const vector<question>& questions, const string& path, bool append = true)
	{
		vector<string> data;

		for (auto q : questions)
		{
			data.push_back(q.ToString());
		}
		save_to_database(data, path, append);
	}

	void answer_question(const int logged_user_id)
	{
		int q_id;
		cout << "Enter Questions ID or -1 to cancel: ";
		cin >> q_id;

		if (q_id == -1)
		{
			cout << "Cancelled\n\n";
			return;
		}

		load_questions_database();

		for (auto& q : questions)
		{
			if (q.get_question_id() == q_id && q.get_receiver_id() == logged_user_id)
			{
				cout << "Question ID (" << q.get_question_id() << ") ";
				cout << "From User ID (" << q.get_sender_id() << ")\t\t";
				cout << "Question: " << q.get_question_body() << "\n";
				if (q.get_answer_body() != "")
				{
					cout << "\tAnswer: " << q.get_answer_body() << "\n\n";
					cout << "Warning: already answered. Answer will be updated!\n";
				}
				cout << "Enter answer: ";
				cin.ignore();
				string answer;
				getline(cin, answer);
				q.set_answer_body(answer);

				question_save_to_database(questions,"questions.txt",false);
				cout << "\n";
				return;
			}
		}
		cout << "There is no question with this ID or It's not asked to you\n\n";
	}
	void delete_question(const int logged_user_id)
	{
		cout << "Enter Question ID or -1 to cancel: ";
		int q_id;
		cin >> q_id;

		if (q_id == -1)
		{
			cout << "Cancelled\n\n";
			return;
		}

		load_questions_database();

		for (auto& q : questions)
		{
			if (q.get_question_id() == q_id && q.get_sender_id() == logged_user_id)
			{
				delete_threads(q.get_question_id());
				q.set_question_Id(-1);

				vector<question> updated_questions;
				for (auto q : questions)
				{
					if (q.get_question_id() != -1)
						updated_questions.push_back(q);
				}
				question_save_to_database(updated_questions,"questions.txt",false);
				cout << "Deleted\n\n";
				return;
			}
		}
		cout << "There is no questions with such ID or it's not your question\n\n";
	}
	void delete_threads(const int& q_id)
	{
		int thread_of_thread_id = 0;
		for (auto& q : questions)
		{
			if (q_id == q.get_question_id())
				continue;
			if (q_id == q.get_parent_id() || thread_of_thread_id == q.get_parent_id())
			{
				thread_of_thread_id = q.get_question_id();
				q.set_question_Id(-1);
			}
			else
				return;	
		}
	}
	void ask_new_question( question& question)
	{
		string question_body;
		cout << "Enter Question text: ";
		cin.ignore();
		getline(cin, question_body);

		question.set_question_body(question_body);
		questions.push_back(question);
		question_save_to_database(questions, "questions.txt");

		cout << "Question successfully asked\n\n";
	}
	void ask_thread(question& n_question)
	{
		string question_body;
		vector<question> threads;
		vector<question> threaded_questions;
		load_questions_database();

		for (int i = 0; i < questions.size(); i++)
		{
			if (questions[i].get_question_id() == n_question.get_parent_id())
			{
				for (int j = i + 1; j < questions.size(); j++)
				{
					if (questions[j].get_parent_id() == n_question.get_parent_id())
						threads.push_back(questions[j]);
					else
						break;
				}

				cout << "Enter Question text: ";
				cin.ignore();
				getline(cin, question_body);
				n_question.set_question_body(question_body);
				threads.push_back(n_question);


				for (auto q : questions)
				{
					if (q.get_question_id() == n_question.get_parent_id())
					{
						threaded_questions.push_back(q);
						for (auto t : threads)
						{
							threaded_questions.push_back(t);
						}
					}
					else
					{
						if (q.get_parent_id() == n_question.get_parent_id())
							continue;
						threaded_questions.push_back(q);
					}
				}

				question_save_to_database(threaded_questions, "questions.txt", false);
				return;
			}
		}
		cout << "There is no questions with such ID\n\n";
	}
	void feed()
	{
		load_questions_database();

		for (auto question : questions)
		{
			if (question.get_parent_id() != -1)
			{
				cout << "\tThread: Parent Question ID (" << question.get_parent_id() << ") ";
			}
			cout << "Question ID (" << question.get_question_id() << ") ";
			if (!question.get_is_asked_anonymously())
			{
				cout << "from User ID (" << question.get_sender_id() << ") ";
			}
			cout << "to User ID (" << question.get_receiver_id() << ")\t\t";
			cout << "\tQuestion: " << question.get_question_body() << "\n";
			if (question.get_answer_body() != "")
			{
				cout << "\tAnswer: " << question.get_answer_body() << "\n";
			}
		}
		cout << "\n";
	}
};
class Ask_FM
{
private:
	users_manager users;
	questions_manager questions;
	user current_logged_user;

public:
	
	void run(bool& run)
	{
		vector<string> choices;
		choices.push_back("Login");
		choices.push_back("SignUp");
		choices.push_back("Close Program");

		int choice = menu(choices);

		if (choice == 1)
		{
			users.load_users_database();
			if (!current_logged_user.login(users.get_users()))
				return;
		}
		else if (choice == 2)
		{
			users.load_users_database();
			if (!current_logged_user.signup(users.get_users(), users.get_last_user_id()++))
				return;
			else
			{
				vector<string> data;
				data.push_back(current_logged_user.To_String());
				save_to_database(data, "users.txt");
			}	
		}
		else if (choice == 3)
		{
			cout << "Goodbye!" << "\n";
			run = false;
			return;
		}

		choices.clear();
		choices.push_back("Print questions to me");
		choices.push_back("Print questions from me");
		choices.push_back("Answer a question");
		choices.push_back("Delete a question");
		choices.push_back("Ask a question");
		choices.push_back("List system users");
		choices.push_back("Feed");
		choices.push_back("Logout");

		while (true)
		{
			choice = menu(choices);

			if (choice == 1)
			{
				questions.load_questions_database();
				current_logged_user.print_q_to_me(questions.get_questions());
			}
			else if (choice == 2)
			{
				questions.load_questions_database();
				current_logged_user.print_q_from_me(questions.get_questions());
			}
			else if (choice == 3)
			{
				questions.answer_question(current_logged_user.get_user_id());
			}
			else if (choice == 4)
			{
				questions.delete_question(current_logged_user.get_user_id());
			}
			else if (choice == 5)
			{
				ask_question();
			}
			else if (choice == 6)
			{
				users.list_system_users();
			}
			else if (choice == 7)
			{
				questions.feed();
			}
			else if (choice == 8)
			{
				cout << "Logged out!\n\n";
				break;
			}
		}
	}
	const int& menu(const vector<string>& choices)
	{
		cout << "Menu: \n";
		for (int i = 0; i < choices.size(); i++)
		{
			cout << "\t" << i + 1 << ": " << choices[i] << "\n";
		}
		
		const int choice = read_choice(1, choices.size());
		return choice;
	}
	const int read_choice(const int low, const int high)
	{
		cout << "\nEnter number in range 1 - " << high << ": ";

		int choice;
		cin >> choice;

		if (choice<low || choice>high)
		{
			cout << "Invalid input...\n\n";
			read_choice(low, high);
		}
		return choice;
	}

	void ask_question()
	{
		int receiver_id, parent_id;
		string question_body;
		bool ask_anonymously;

		cout << "Enter User ID or -1 to cancel: ";
		cin >> receiver_id;

		if (receiver_id == -1)
		{
			cout << "Cancelled!\n\n";
			return;
		}

		users.load_users_database();

		for (auto user : users.get_users())
		{
			if (user.get_user_id() == receiver_id)
			{
				if (!user.get_accepts_anonymous_questions())
				{
					cout << "Anonymous Questions are not allowed for this user!\n";
					ask_anonymously = 0;
				}
				else
				{
					cout << "Would you like to ask anonymously? (0 or 1): ";
					cin >> ask_anonymously;
				}

				cout << "For Thread Questions: Enter Question ID or -1 for new Question: ";
				cin >> parent_id;

				if (parent_id < -1||parent_id==0)
				{
					cout << "Invalid ID...\n\n";
					return;
				}

				question question(questions.get_last_question_id()++, parent_id, current_logged_user.get_user_id(), receiver_id, ask_anonymously);
				if (parent_id == -1)
				{
					questions.ask_new_question(question);
					return;
				}
				else
				{
					questions.ask_thread(question);
					return;
				}
				cout << "\n";
			}
		}
		cout << "User not found!\n\n";
	}
};

int main(void)
{
	bool run = true;

	Ask_FM system;

	while (run)
	{
		system.run(run);
	}
	return 0;
}