#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <limits>
#include <vector>

using namespace std;

int get_int(void);
float get_float(void);
void create_page(void);
void query_page(void);
void format_page(void);
void more_page(void);
void search_keyword(void);
bool check_exist(const int);

struct transaction_record
{
	int id;
	string sender;
	string receiver;
	float amount;
	string remarks;
	string status;
};

struct inventory_record
{
	int id;
	string name;
	float price;
	int stock;
	string demand;
	int exp_date;
};

enum ynn {y, n, invalid};
unordered_map<string, ynn> for_save = {{"y", y}, {"n", n}}; 

class log
{
	public:
		virtual void display() = 0;
		virtual void save_record() = 0;
		virtual ~log() {}
};

class transaction_log : public log
{
	transaction_record record;
	
	public:
		transaction_log(const transaction_record& temp)
		{
			this->record = temp;
		}
		
		void display() override
		{
			cout << "----------------------------";
			cout << "\nTransaction No.: " << record.id 		 << endl;
			cout << "Sender: " 			  << record.sender 	 << endl;
			cout << "Receiver: " 		  << record.receiver << endl;
			cout << "Amount: " 			  << record.amount 	 << endl;
			cout << "Remarks: " 		  << record.remarks  << endl;
			cout << "Status: " 			  << record.status   << endl;
			cout << "----------------------------";
		}
		
		void save_record() override
		{
			ofstream file("Transaction_Log.txt", ios::out | ios::app | ios::binary);
			if(!file)
			{
				cerr << "\nerror! failed to open file.\n";
			}
			else
			{
				file.write(reinterpret_cast<const char*>(&record.id), sizeof(record.id));
				file.write(reinterpret_cast<const char*>(&record.amount), sizeof(record.amount));
				
				auto write_string = [&file](const string& str)
				{
					size_t length = str.size();
					file.write(reinterpret_cast<const char*>(&length), sizeof(length));
					if(length > 0)
					{
						file.write(str.data(), length);
					}
				};
				write_string(record.sender);
				write_string(record.receiver);
				write_string(record.remarks);
				write_string(record.status);
				
				file.close();
				cout << "\nRecord saved to Log successfully.\n";
			}				
		}
		
		static void read_record(const ynn& read)
		{
			int target_id;
			switch(read)
			{
				case y:
				{
					ifstream file("Transaction_Log.txt", ios::binary | ios::in);
					if(!file)
					{
						cerr << "\nerror! failed to open file.\n";
					}
					else
					{
						transaction_record temp;
						
						auto read_string = [&file](string& str) -> bool
						{
							size_t length = 0;
							if(!file.read(reinterpret_cast<char*>(&length), sizeof(length))) return false;
							str.resize(length);
							if(length > 0)
							{
								file.read(&str[0], length);
							}
							return true;
						};
						cout << "----------------------------";
						while(file.read(reinterpret_cast<char*>(&temp.id), sizeof(temp.id)))
						{
							file.read(reinterpret_cast<char*>(&temp.amount), sizeof(temp.amount));
							read_string(temp.sender);
							read_string(temp.receiver);
							read_string(temp.remarks);
							read_string(temp.status);
							
							cout << "\nTransaction No.: " << temp.id 	   << endl;
							cout << "Sender: " 			  << temp.sender   << endl;
							cout << "Receiver: " 		  << temp.receiver << endl;
							cout << "Amount: " 			  << temp.amount   << endl;
							cout << "Remarks: " 		  << temp.remarks  << endl;
							cout << "Status: " 			  << temp.status   << endl;
						}
						cout << "----------------------------";
						file.close();
					}
					break;
				}
				case n:
				{
					cout << "\nFor specific query, enter the transaction no.: ";
					target_id = get_int();
					
					ifstream file("Transaction_Log.txt", ios::binary | ios::in);
					if(!file)
					{
						cerr << "\nerror! failed to open file.\n";
					}
					else
					{
						unordered_map<int, streampos> index;
						
						auto read_string = [&file](string& str) -> bool
						{
							size_t length = 0;
							if(!file.read(reinterpret_cast<char*>(&length), sizeof(length))) return false;
							str.resize(length);
							if(length > 0)
							{
								file.read(&str[0], length);
							}
							return true;
						};
						
						transaction_record temp;
						while(true)
						{
							streampos position = file.tellg();
							if(!file.read(reinterpret_cast<char*>(&temp.id), sizeof(temp.id))) break;
							if(!file.read(reinterpret_cast<char*>(&temp.amount), sizeof(temp.amount))) break;
							if(!read_string(temp.sender)) break;
							if(!read_string(temp.receiver)) break;
							if(!read_string(temp.remarks)) break;
							if(!read_string(temp.status)) break;
							
							if(index.find(temp.id) == index.end())
							{
								index[temp.id] = position;
							}
						}
						
						auto x = index.find(target_id);
						if(x == index.end())
						{
							cout << "\nEntered id not found in Log.\n";
						}
						else
						{
							streampos offset = x->second;
							file.clear();
							file.seekg(offset);
							transaction_record temp1;
							
							file.read(reinterpret_cast<char*>(&temp1.id), sizeof(temp1.id));
							file.read(reinterpret_cast<char*>(&temp1.amount), sizeof(temp1.amount));
							read_string(temp1.sender);
							read_string(temp1.receiver);
							read_string(temp1.remarks);
							read_string(temp1.status);
							
							cout << "----------------------------";
							cout << "\nTransaction No.: " << temp1.id 	   << endl;
							cout << "Sender: " 			  << temp1.sender   << endl;
							cout << "Receiver: " 		  << temp1.receiver << endl;
							cout << "Amount: " 			  << temp1.amount   << endl;
							cout << "Remarks: " 		  << temp1.remarks  << endl;
							cout << "Status: " 			  << temp1.status   << endl;
							cout << "----------------------------";
							file.close();
						}	
					
					break;	
					}
				}
				default:
					cout << "\ninvalid input!\n";
					break;						
			}
		}
};

class inventory_log : public log
{
	inventory_record record;
	
	public:
		inventory_log(const inventory_record& temp)
		{
			this->record = temp;
		}
		
		void display() override
		{
			cout << "----------------------------";
			cout << "\nProduct No.: " << record.id 	       << endl;
			cout << "Name: " 		  << record.name       << endl;
			cout << "Price: " 		  << record.price      << endl;
			cout << "Stocks: " 		  << record.stock 	   << endl;
			cout << "Demand: " 		  << record.demand     << endl;
			cout << "Exp. Date: " 	  << record.exp_date   << endl;
			cout << "----------------------------";
		}
		
		void save_record() override
		{
			ofstream file("Inventory_Log.txt", ios::out | ios::app | ios::binary);
			if(!file)
			{
				cerr << "\nerror! failed to open file.\n";
			}
			else
			{
				file.write(reinterpret_cast<const char*>(&record.id), sizeof(record.id));
				file.write(reinterpret_cast<const char*>(&record.price), sizeof(record.price));
				file.write(reinterpret_cast<const char*>(&record.stock), sizeof(record.stock));
				file.write(reinterpret_cast<const char*>(&record.exp_date), sizeof(record.exp_date));
				
				auto write_string = [&file](const string& str)
				{
					size_t length = str.size();
					file.write(reinterpret_cast<const char*>(&length), sizeof(length));
					if(length > 0)
					{
						file.write(str.data(), length);
					}
				};
				write_string(record.name);
				write_string(record.demand);
				
				file.close();
				cout << "\nRecord saved to Log successfully.\n";
			}				
		}
		
		static void read_record(const ynn& read)
		{
			int target_id;
			switch(read)
			{
				case y:
				{
					ifstream file("Inventory_Log.txt", ios::binary | ios::in);
					if(!file)
					{
						cerr << "\nerror! failed to open file.\n";
					}
					else
					{
						inventory_record temp;
						
						auto read_string = [&file](string& str) -> bool
						{
							size_t length = 0;
							if(!file.read(reinterpret_cast<char*>(&length), sizeof(length))) return false;
							str.resize(length);
							if(length > 0)
							{
								file.read(&str[0], length);
							}
							return true;
						};
						cout << "----------------------------";
						while(file.read(reinterpret_cast<char*>(&temp.id), sizeof(temp.id)))
						{
							file.read(reinterpret_cast<char*>(&temp.price), sizeof(temp.price));
							file.read(reinterpret_cast<char*>(&temp.stock), sizeof(temp.stock));
							file.read(reinterpret_cast<char*>(&temp.exp_date), sizeof(temp.exp_date));
							read_string(temp.name);
							read_string(temp.demand);
							
							cout << "\nProduct No.: " << temp.id 	     << endl;
							cout << "Name: " 		  << temp.name       << endl;
							cout << "Price: " 		  << temp.price      << endl;
							cout << "Stocks: " 		  << temp.stock 	 << endl;
							cout << "Demand: " 		  << temp.demand     << endl;
							cout << "Exp. Date: " 	  << temp.exp_date   << endl;
						}
						cout << "----------------------------";
						file.close();
					}
					break;
				}
				case n:
				{
					cout << "\nFor specific query, enter the Product ID: ";
					target_id = get_int();
					
					ifstream file("Inventory_Log.txt", ios::binary | ios::in);
					if(!file)
					{
						cerr << "\nerror! failed to open file.\n";
					}
					else
					{
						unordered_map<int, streampos> index;
						
						auto read_string = [&file](string& str) -> bool
						{
							size_t length = 0;
							if(!file.read(reinterpret_cast<char*>(&length), sizeof(length))) return false;
							str.resize(length);
							if(length > 0)
							{
								file.read(&str[0], length);
							}
							return true;
						};
						
						inventory_record temp;
						while(true)
						{
							streampos position = file.tellg();
							if(!file.read(reinterpret_cast<char*>(&temp.id), sizeof(temp.id))) break;
							if(!file.read(reinterpret_cast<char*>(&temp.price), sizeof(temp.price))) break;	
							if(!file.read(reinterpret_cast<char*>(&temp.stock), sizeof(temp.stock))) break;	
							if(!file.read(reinterpret_cast<char*>(&temp.exp_date), sizeof(temp.exp_date))) break;
							if(!read_string(temp.name)) break;
							if(!read_string(temp.demand)) break;
							
							if(index.find(temp.id) == index.end())
							{
								index[temp.id] = position;
							}
						}
						
						auto x = index.find(target_id);
						if(x == index.end())
						{
							cout << "\nEntered id not found in Log.\n";
						}
						else
						{
							streampos offset = x->second;
							file.clear();
							file.seekg(offset);
							inventory_record temp1;
							
							file.read(reinterpret_cast<char*>(&temp1.id), sizeof(temp1.id));
							file.read(reinterpret_cast<char*>(&temp1.price), sizeof(temp1.price));
							file.read(reinterpret_cast<char*>(&temp1.stock), sizeof(temp1.stock));
							file.read(reinterpret_cast<char*>(&temp1.exp_date), sizeof(temp1.exp_date));
							read_string(temp1.name);
							read_string(temp1.demand);
							
							cout << "----------------------------";
							cout << "\nProduct No.: " << temp1.id 	     << endl;
							cout << "Name: " 		  << temp1.name       << endl;
							cout << "Price: " 		  << temp1.price      << endl;
							cout << "Stocks: " 		  << temp1.stock 	 << endl;
							cout << "Demand: " 		  << temp1.demand     << endl;
							cout << "Exp. Date: " 	  << temp1.exp_date   << endl;
							cout << "----------------------------";
							file.close();
						}	
					
					break;	
					}
				}
				default: 
					cout << "\ninvalid input!\n"; 
					break;						
			}
		}
};

int main()
{
	enum main_options {e_create, e_query, e_format, e_more, e_exit, invalid};	
	unordered_map<string, main_options> action = {
		{"create", e_create},
		{"query", e_query},
		{"format", e_format},
		{"more", e_more},
		{"exit", e_exit}};
		
	string main_user_input;
	cout << "\n---Welcome to this simple database for simple records---" << endl;
	
	do
	{
		cout << "\nWhat do you want to do?\n"
			 << "(create, query, format, more, exit): ";
		getline(cin, main_user_input);
		
		auto x = action.find(main_user_input);
		main_options input = (x != action.end()) ? x->second : invalid; 
		
		switch(input)
		{
			case e_create:
				create_page();
				break;
				
			case e_query:
				query_page();
				break;
				
			case e_format:
				format_page();
				break;
				
			case e_more:
				more_page();
				break;
				
			case e_exit:
				cout << "\nShutting down system....Done!\n";
				break;
				
			default:
				cerr << "\ninvalid input! try again." << endl;
		}
	}while(main_user_input != "exit");
	
	return 0;
}

int get_int()
{
	int value;
	while(true)
	{
		if(cin >> value)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return value;
		}
		else
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cerr << "\ninvalid input!\nEnter numeric value: ";
		}
	}
}

float get_float()
{
	float value;
	while(true)
	{
		if(cin >> value)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return value;
		}
		else
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cerr << "\ninvalid input!\nEnter numeric value: ";
		}
	}
}

void create_page()
{
	int user_input;
	string user_input_save;
	do
	{
		cout << "\nWhat do you want to create?\n"
			 << "(Transaction Record = 1, Inventory Record = 2, Exit = 0): ";
		user_input = get_int();
		
		switch(user_input)
		{
			case 0:
				cout << "\nExiting...\n" << endl;
				break;
			
			case 1:
				{
					transaction_record* temp = new transaction_record();	
					bool is_there;				
					do
					{
						cout << "\nTransaction No.: "; temp->id = get_int();
						is_there = check_exist(temp->id);
					}while(is_there);
					//cout << "\nTransaction No.: "; temp->id = get_int();		
					cout << "Sender: "; 		   getline(cin, temp->sender);
					cout << "Receiver: "; 	 	   getline(cin, temp->receiver);
					cout << "Amount: "; 		   temp->amount = get_float();
					cout << "Remarks: "; 		   getline(cin, temp->remarks);
					cout << "Status: "; 		   getline(cin, temp->status);
					
					transaction_log t(*temp);
					delete temp;
					cout << "\nRecord Preview: \n";
					t.display();
					
					cout << "\nDo you want to save this record to log? (y/n): ";
					getline(cin,  user_input_save);
					auto x = for_save.find(user_input_save);
					ynn save = (x != for_save.end()) ? x->second : invalid;
					
					switch(save)
					{
						case y: {t.save_record();	break;}
						case n: cout << "\nOk, record not saved to the log.\n";	break;
						default: cout << "\ninvalid input! record not saved to the log.\n"; break;						
					}
					break;
				}
				
			case 2:
				{
					inventory_record* temp = new inventory_record();
					
					cout << "\nProduct No.: "; temp->id = get_int();
					cout << "Name: "; 		   getline(cin, temp->name);
					cout << "Price: "; 		   temp->price = get_float();
					cout << "Stocks: "; 	   temp->stock = get_int();
					cout << "Demand: "; 	   getline(cin, temp->demand);
					cout << "Exp. Date: "; 	   temp->exp_date = get_int();
					
					inventory_log i(*temp);
					delete temp;
					cout << "\nRecord Preview: \n";
					i.display();
					
					cout << "\nDo you want to save this record to log? (y/n): ";
					getline(cin,  user_input_save);
					auto x = for_save.find(user_input_save);
					ynn save = (x != for_save.end()) ? x->second : invalid;
					
					switch(save)
					{
						case y: {i.save_record();	break;}
						case n: cout << "\nOk, record not saved to the log.\n";	break;
						default: cout << "\ninvalid input! record not saved to the log.\n"; break;						
					}
					break;
				}
								
			default:
				cerr << "\ninvalid input! try again." << endl;
		}
	}while(user_input != 0);
}

void query_page()
{
	int user_input;
	string user_input_read;
	do
	{
		cout << "\nWhat do you want to read?\n"
			 << "(Transaction Log = 1, Inventory Log = 2, Exit = 0): ";
		user_input = get_int();
		
		switch(user_input)
		{
			case 0:
				cout << "\nExiting...\n" << endl;
				break;
			
			case 1:
				{
					cout << "\nDo you want to read the entire log? (y/n): ";
					getline(cin,  user_input_read);
					auto x = for_save.find(user_input_read);
					ynn read = (x != for_save.end()) ? x->second : invalid;
					
					transaction_log::read_record(read);
					break;
				}
				
			case 2:
				{
					cout << "\nDo you want to read the entire log? (y/n): ";
					getline(cin,  user_input_read);
					auto x = for_save.find(user_input_read);
					ynn read = (x != for_save.end()) ? x->second : invalid;
					
					inventory_log::read_record(read);
					break;
				}
								
			default:
				cerr << "\ninvalid input! try again." << endl;
		}
	}while(user_input != 0);
}

void format_page()
{
	int user_input;
	cout << "\nWhat do you want to format?\n"
		 << "(Transaction Log = 1, Inventory Log = 2, Exit = 0): ";
	user_input = get_int();
	
	switch(user_input)
	{
		case 0:
			cout << "\nExiting...\n" << endl;
			break;
		
		case 1:{
			string user_ans;
			cout << "\n(Formatting will delete all records inside Transaction_Log.txt!)\n";
			cout << "Are you sure you want to continue. (y/n): ";
			getline(cin, user_ans);
			
			auto x = for_save.find(user_ans);
			ynn conform = (x != for_save.end())	? x->second : invalid;
			
			switch(conform)
			{
				case y:{
					ofstream file("Transaction_Log.txt", ios::out | ios::trunc | ios::binary);
					if(file.is_open())
					{
						cout << "\nfile formatted successfully.\n";
						file.close();
					}
					else
					{
						cerr << "\nerror! file not formatted.\n";
					}
					break;}
					
				case n:
					cout << "\nClose call..almost lost all data..\n";
					break;
					
				default:
					cerr << "\ninvalid input! try again." << endl;	
			}
			break;}
			
		case 2:{
			string user_ans;
			cout << "\n(Formatting will delete all records inside Inventory_Log.txt!)\n";
			cout << "Are you sure you want to continue. (y/n): ";
			getline(cin, user_ans);
			
			auto x = for_save.find(user_ans);
			ynn conform = (x != for_save.end())	? x->second : invalid;
			
			switch(conform)
			{
				case y:{
					ofstream file("Inventory_Log.txt", ios::out | ios::trunc | ios::binary);
					if(file.is_open())
					{
						cout << "\nfile formatted successfully.\n";
						file.close();
					}
					else
					{
						cerr << "\nerror! file not formatted.\n";
					}
					break;}
					
				case n:
					cout << "\nClose call..almost lost all data..\n";
					break;
					
				default:
					cerr << "\ninvalid input! try again." << endl;	
			}
			break;}
			
		default:
			cerr << "\ninvalid input! try again." << endl;
	}	 
}

void more_page()
{
	int user_input;
	do
	{
		cout << "\nWhat other things do you want to do?\n"
			 << "(See files size = 1, Search keyword = 2, Exit = 0): ";
		user_input = get_int();
		
		switch(user_input)
		{
			case 0:
				cout << "\nExiting...\n" << endl;
				break;
				
			case 1:
			{
				streampos t_size, i_size;
				ifstream tfile("Transaction_Log.txt", ios::binary | ios::in);
				tfile.seekg(0, ios::end);
				t_size = tfile.tellg();
				cout << "----------------------------";
				cout << "\nTransaction_Log: " << t_size << "bytes.\n";
				tfile.close();
				ifstream ifile("Inventory_Log.txt", ios::binary | ios::in);
				ifile.seekg(0, ios::end);
				i_size = ifile.tellg();
				cout << "Inventory_Log: " << i_size << "bytes.\n";
				cout << "----------------------------";
				ifile.close();
				break;
			}
			
			case 2:
			{
				search_keyword();				
				break;
			}
			
			default:
				cerr << "\ninvalid input! try again." << endl;
		}
	}while(user_input != 0);
}

void search_keyword()
{
				string keyword;
				int for_file;
				cout << "\nEnter the keyword to search: ";
				getline(cin, keyword);
				cout << "\nSearch in (Transaction Log = 1, Inventory Log = 2): ";
				for_file = get_int();
				
				switch(for_file)
				{
					case 1:
					{			
						ifstream file("Transaction_Log.txt", ios::in | ios::binary);
						if(!file)
						{
							cerr << "\nerror! failed to open file.\n";
						}
						else
						{
							unordered_map<string, vector<streampos>> index;
							
							auto read_string = [&file](string& str) -> bool
							{
								size_t length = 0;
								if(!file.read(reinterpret_cast<char*>(&length), sizeof(length))) return false;
								str.resize(length);
								if(length > 0)
								{
									file.read(&str[0], length);
								}
								return true;
							};
							
							transaction_record temp;
							while(true)
							{
								streampos position = file.tellg();
								
								if(!file.read(reinterpret_cast<char*>(&temp.id), sizeof(temp.id))) break;
								if(!file.read(reinterpret_cast<char*>(&temp.amount), sizeof(temp.amount))) break;
								if(!read_string(temp.sender)) break;
								if(!read_string(temp.receiver)) break;
								if(!read_string(temp.remarks)) break;
								if(!read_string(temp.status)) break;
								
								index[to_string(temp.id)].push_back(position);
								index[to_string(temp.amount)].push_back(position);
								index[temp.sender].push_back(position);
								index[temp.receiver].push_back(position);
								index[temp.remarks].push_back(position);
								index[temp.status].push_back(position);
							}
							
							auto x = index.find(keyword);
							if(x != index.end())
							{
								cout << "\nFound '" << x->second.size() << "' records with keyword\n";
								for(streampos offset : x->second)
								{
									file.clear();
									file.seekg(offset);
									
									transaction_record temp1;
									
									file.read(reinterpret_cast<char*>(&temp1.id), sizeof(temp1.id));
									file.read(reinterpret_cast<char*>(&temp1.amount), sizeof(temp1.amount));
									read_string(temp1.sender);
									read_string(temp1.receiver);
									read_string(temp1.remarks);
									read_string(temp1.status);
									
									cout << "----------------------------";
									cout << "\nTransaction No.: " << temp1.id 	   << endl;
									cout << "Sender: " 			  << temp1.sender   << endl;
									cout << "Receiver: " 		  << temp1.receiver << endl;
									cout << "Amount: " 			  << temp1.amount   << endl;
									cout << "Remarks: " 		  << temp1.remarks  << endl;
									cout << "Status: " 			  << temp1.status   << endl;
								}
								cout << "----------------------------";
							}
							else
							{
								cout << "\nEntered keyword not found in entire Log.\n";
							}
						}
					}
					
					case 2:
					{
						ifstream file("Inventory_Log.txt", ios::in | ios::binary);
						if(!file)
						{
							cerr << "\nerror! failed to open file.\n";
						}
						else
						{
							unordered_map<string, vector<streampos>> index;
							
							auto read_string = [&file](string& str) -> bool
							{
								size_t length = 0;
								if(!file.read(reinterpret_cast<char*>(&length), sizeof(length))) return false;
								str.resize(length);
								if(length > 0)
								{
									file.read(&str[0], length);
								}
								return true;
							};
							
							inventory_record temp;
							while(true)
							{
								streampos position = file.tellg();
								
								if(!file.read(reinterpret_cast<char*>(&temp.id), sizeof(temp.id))) break;
								if(!file.read(reinterpret_cast<char*>(&temp.price), sizeof(temp.price))) break;
								if(!file.read(reinterpret_cast<char*>(&temp.stock), sizeof(temp.stock))) break;
								if(!file.read(reinterpret_cast<char*>(&temp.exp_date), sizeof(temp.exp_date))) break;
								if(!read_string(temp.name)) break;
								if(!read_string(temp.demand)) break;
								
								index[to_string(temp.id)].push_back(position);
								index[to_string(temp.price)].push_back(position);
								index[to_string(temp.stock)].push_back(position);
								index[to_string(temp.exp_date)].push_back(position);
								index[temp.demand].push_back(position);
								index[temp.name].push_back(position);
							}
							
							auto x = index.find(keyword);
							if(x != index.end())
							{
								cout << "\nFound '" << x->second.size() << "' records with keyword\n";
								for(streampos offset : x->second)
								{
									file.clear();
									file.seekg(offset);
									
									inventory_record temp1;
									
									file.read(reinterpret_cast<char*>(&temp1.id), sizeof(temp1.id));
									file.read(reinterpret_cast<char*>(&temp1.price), sizeof(temp1.price));
									file.read(reinterpret_cast<char*>(&temp1.stock), sizeof(temp1.stock));
									file.read(reinterpret_cast<char*>(&temp1.exp_date), sizeof(temp1.exp_date));
									read_string(temp1.name);
									read_string(temp1.demand);
									
									cout << "----------------------------";
									cout << "\nProduct No.: " << temp1.id 	     << endl;
									cout << "Name: " 		  << temp1.name       << endl;
									cout << "Price: " 		  << temp1.price      << endl;
									cout << "Stocks: " 		  << temp1.stock 	 << endl;
									cout << "Demand: " 		  << temp1.demand     << endl;
									cout << "Exp. Date: " 	  << temp1.exp_date   << endl;
								}
								cout << "----------------------------";
							}
							else
							{
								cout << "\nEntered keyword not found in entire Log.\n";
							}
						}
					}
				}
}

bool check_exist(const int id)
{
	ifstream file("Transaction_Log.txt", ios::binary | ios::in);
	if(!file) return false;
	
	transaction_record temp;

	auto read_string = [&file](string& str) -> bool
	{
		size_t length = 0;
		if(!file.read(reinterpret_cast<char*>(&length), sizeof(length))) return false;
		str.resize(length);
		if(length > 0)
		{
			file.read(&str[0], length);
		}
		return true;
	};
	
	while(file.read(reinterpret_cast<char*>(&temp.id), sizeof(temp.id)))
	{
		file.read(reinterpret_cast<char*>(&temp.amount), sizeof(temp.amount));
		
		if(!read_string(temp.sender)) break;
		if(!read_string(temp.receiver)) break;
		if(!read_string(temp.remarks)) break;
		if(!read_string(temp.status)) break;
		
		if(temp.id == id)
		{
			cerr << "\nThis ID already exists, try again.\n";
			file.close();
            return true;
		}
	}
	
	file.close();
	return false;					
}
