#pragma once
#include<string>
#include<vector>
#include<sstream>

class question
{
private:
	int question_id;
	int sender_id;
	int receiver_id;
	int parent_id;
	bool is_asked_anonymously;
	std::string question_body;
	std::string answer_body;

public:

	question();
	question(const int question_id, const int parent_id, const int sender_id, const int receiver_id, const bool ask_anonymously);
	question(const std::vector<std::string>& data);

	const int get_question_id()const;
	void set_question_Id(const int& question_id);

	const int get_parent_id()const;

	const int get_sender_id()const;

	const int get_receiver_id()const;

	const bool get_is_asked_anonymously()const;

	const std::string& get_question_body()const;
	void set_question_body(const std::string& question_body);

	const std::string& get_answer_body()const;
	void set_answer_body(const std::string& answer);

	const std::string ToString();
};