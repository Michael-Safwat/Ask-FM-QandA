#include"question.h"

question::question()
{

}
question::question(const int question_id, const int parent_id, const int sender_id, const int receiver_id, const bool ask_anonymously)
	:question_id(question_id), parent_id(parent_id), sender_id(sender_id), receiver_id(receiver_id),is_asked_anonymously(ask_anonymously)
{
	question_body = "";
	answer_body = "";
}
question::question(const std::vector<std::string>& data)
{
	question_id = stoi(data[0]);
	parent_id = stoi(data[1]);
	sender_id = stoi(data[2]);
	receiver_id = stoi(data[3]);
	is_asked_anonymously = stoi(data[4]);
	question_body = data[5];

	if (data.size() == 7)
		answer_body = data[6];
}

const int question::get_question_id()const
{
	return question_id;
}
void question::set_question_Id(const int& question_id)
{
	this->question_id = question_id;
}


const int question::get_parent_id()const
{
	return parent_id;
}

const int question::get_sender_id()const
{
	return sender_id;
}

const int question::get_receiver_id()const
{
	return receiver_id;
}

const bool question::get_is_asked_anonymously()const
{
	return is_asked_anonymously;
}

const std::string& question::get_question_body()const
{
	return question_body;
}
void question::set_question_body(const std::string& question_body)
{
	this->question_body = question_body;
}


const std::string& question::get_answer_body()const
{
	return answer_body;
}
void question::set_answer_body(const std::string& answer)
{
	this->answer_body = answer;
}

const std::string question::ToString()
{
	std::ostringstream oss;
	oss << get_question_id() << "~" << get_parent_id() << "~" << get_sender_id() << "~" << get_receiver_id() << "~"
		<< get_is_asked_anonymously() << "~" << get_question_body();

	if (get_answer_body() != "")
		oss << "~" << get_answer_body();

	return oss.str();
}