#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<unordered_map>
using namespace std;

void read_file_full(const string& file_name);
void read_file_window(const string& file_name);
void file_format(const string& file_name);
void file_delete(string file_name);

class record
{
	public:
		virtual ~record(){}
		virtual void display() = 0;
		virtual void save_file() = 0;
};

class transaction_record : public record
{
	int index;
	int sender;
	int receiver;
	float amount;
	string remarks;	

	public:
		transaction_record()
		{
			cout << "Transaction No.: ";
			cin >> index;
			cout << "Sender: ";
			cin >> sender;
			cout << "Receiver: ";
			cin >> receiver;
			cout << "Amount: ";
			cin >> amount;
			cout << "Remarks(1 word only): ";
			cin >> remarks;			
		}

		void display() override
		{
			cout << "Transaction No.: " << index << endl
				 << "Sender: " << sender << endl
				 << "Receiver: " << receiver << endl
				 << "Amount: " << amount << endl
				 << "Remarks: " << remarks << endl;
		}

		void save_file() override
		{
			string file_name;
			cout << "Save this log to file: ";
			cin >> file_name;
			ofstream file(file_name, ios::out | ios::binary | ios::app);
			if(file)
			{
				ostringstream ss;
				ss   << "Transaction No.: " << index << endl
					 << "Sender: " 			<< sender << endl
				     << "Receiver: " 		<< receiver << endl
					 << "Amount: " 			<< amount << endl
					 << "Remarks: " 		<< remarks << endl << endl;
				string binary_form = ss.str();
				file.write(binary_form.c_str(), binary_form.size());
				file.close();
				cout << "\nTransaction record saved to " << file_name << ".\n";
			}
			else
			{
				cerr << "\nfailed to save data to disk. error!\n"; 
			}
		}
};

class inventory_record : public record
{
	int index;
	string item;
	float price;
	int stock;
	string pop;	
	public:

		inventory_record()
		{
			cout << "item No.: ";
			cin >> index;
			cout << "Item: ";
			cin >> item;
			cout << "Price: ";
			cin >> price;
			cout << "Stock: ";
			cin >> stock;
			cout << "Popularity: ";
			cin >> pop;			
		}

		void display() override
		{
			cout << "Item No.: " << index << endl
				 << "Item: " << item << endl
				 << "Price: " << price << endl
				 << "Stock: " << stock << endl
				 << "Popularity: " << pop << endl;
		}

		void save_file() override
		{
			string file_name;
			cout << "Save this record to file: ";
			cin >> file_name;
			ofstream file(file_name, ios::out | ios::binary | ios::app);
			if(file)
			{
				ostringstream ss;
				ss	 << "Item No.: " 	<< index << endl
					 << "Item: " 		<< item << endl
				     << "Price: " 		<< price << endl
					 << "Stock: " 		<< stock << endl
					 << "Popularity: " 	<< pop << endl << endl;
				string binary_form = ss.str();
				file.write(binary_form.c_str(), binary_form.size());
				file.close();
				cout << "\nInventory record saved to " << file_name << ".\n";
			}
			else
			{
				cerr << "\nfailed to save data to disk. error!\n"; 
			}
		}
};

void create_page()
{
	int user_input, save;

	do
	{
		cout << "\nwhat would you like to create?\n";
		cout << "(Transaction Log = 1, Inventory Record = 2, Exit = 0): ";
		cin >> user_input;
		
		if(user_input == 0)
		{
			cout << "\nExiting...\n";			
		}
		
		else if(user_input == 1)
		{
			cout << endl;
			record* t1 = new transaction_record();

			cout << endl << "Preview: \n\n";
			t1->display();

			cout << "\nWould you like to save this transaction log? (1/0): ";
			cin >> save;

			if(save == 1)
			{
				t1->save_file();
				delete t1;
			}

			else if(save == 0)
			{
				cout << "Ok, info not saved!\n";	
			}		
		}
		
		else if(user_input == 2)
		{
			cout << endl;
			record* ir1 = new inventory_record();

			cout << endl << "Preview: \n\n";
			ir1->display();

			cout << "\nWould you like to save this inventory record? (1/0): ";
			cin >> save;

			if(save == 1)
			{
				ir1->save_file();
				delete ir1;
			}

			else if(save == 0)
			{
				cout << "Ok, info not saved!\n";	
			}
		}
		
		else
		{
			cerr << "\ninvalid input, try again..\n";
		}
		
	}while(user_input != 0);
}

void query_page()
{
	int user_input, read_mode;
	string file_name;
	do
	{
		cout << "\nwhat would you like to read?\n";
		cout << "(Transaction Log = 1, Inventory Record = 2, Exit = 0): ";
		cin >> user_input;
		
		if(user_input == 0)
		{
			cout << "\nExiting...\n";			
		}

		else if(user_input == 1)
		{
			cout << "\nRead from file: ";
			cin >> file_name;
			
			cout << "(Full file read = 1, Window read = 0): ";
			cin >> read_mode;
			try
			{
				if(read_mode != 1 && read_mode != 0) {cerr << "\ninvalid input!\n";}			
				else {(read_mode == 1) ? read_file_full(file_name) : read_file_window(file_name);}
			}
			catch(const runtime_error& e)
			{
				cerr << "\nerror: " << e.what() << endl;
			}	
		}
		
		else if(user_input == 2)
		{
			cout << "\nRead from file: ";
			cin >> file_name;
			
			cout << "(Full file read = 1, Window read = 0): ";
			cin >> read_mode;
			
			try
			{
				if(read_mode != 1 && read_mode != 0) {cerr << "\ninvalid input!\n";}			
				else {(read_mode == 1) ? read_file_full(file_name) : read_file_window(file_name);}
			}
			catch(const runtime_error& e)
			{
				cerr << "\nerror: " << e.what() << endl;
			}
		}
		
		else
		{
			cerr << "\ninvalid input, try again..\n";
		}
				
	}while(user_input != 0);
}

enum action {e_create, e_query, e_format, e_delete, e_quit, e_invalid};

int main()
{
	static const unordered_map<string, action> action_map = 
	{
		{"create", e_create},
		{"query", e_query},
		{"format", e_format},
		{"delete", e_delete},
		{"quit", e_quit}
	}; 

    string user_input, file_name;
    int sure;

    while(user_input != "quit")
    {
        cout << "\nChoose an action (create, query, format, delete, quit): ";
        cin >> user_input;

        auto it = action_map.find(user_input);
        action input = (it != action_map.end()) ? it->second : e_invalid; 

		switch(input)
		{
			case e_create:
				create_page();
				break;

			case e_query:
				query_page();
				break;

			case e_format:
				cout << "File to format: ";
				cin >> file_name;
				cout << "Are you sure.(1/0): ";
				cin >> sure;
				try
				{
					if(sure != 1 && sure != 0) 
					{
						cerr << "\ninvalid input!\n";
					}
					else
					{
						if(sure == 1) file_format(file_name);
						else cout << "\nClose call..\n";
					}	
				}
				catch(const runtime_error& e)
				{
					cerr << "\nerror: " << e.what() << endl;
				}
				break;
			
			case e_delete:
				cout << "File to delete: ";
				cin >> file_name;
				cout << "Are you sure.(1/0): ";
				cin >> sure;
				try
				{
					if(sure != 1 && sure != 0) 
					{
						cerr << "\ninvalid input!\n";
					}
					else
					{
						if(sure == 1) file_delete(file_name);
						else cout << "\nClose call..\n";
					}	
				}
				catch(const runtime_error& e)
				{
					cerr << "\nerror: " << e.what() << endl;
				}				
				break;
			
			case e_quit:
				cout << "\nShutting down system..Done!\n";
				break;
				
			default:
				cout << "\ninvalid input.\n";
				break;
		}
    }

    return 0;
}

void read_file_full(const string& file_name)
{
	string s;
	ifstream read(file_name, ios::in | ios::binary);
	if(!read)
	{
		throw runtime_error("File not found!\n");
	}
	else
	{
		while(getline(read, s))
		{
			cout << s << endl;
		}
		cout << "End of file " << file_name << endl;
	}
	read.close();
}

void read_file_window(const string& file_name)
{
	string s, target, t = "No.: ";
	cout << "\nEnter the Index No.: ";
	cin >> target;
	cout << endl;
	bool found = false;
	int ll, line = 0;
	ifstream read(file_name, ios::in | ios::binary);
	if(!read)
	{
		throw runtime_error("File not found!\n");
	}
	else
	{
		while(getline(read, s))
		{
			line++;
			if(s.find(t) != string::npos && s.find(target) != string::npos && !found)
			{
				found = true;
				ll = line;
			}
			if(found && line < ll + 5)
			{
				cout << s << endl;
			}
		}	
		cout << "\nEnd of file " << file_name << endl;
	}
	read.close();
}

void file_delete(string file_name)
{
	ifstream file(file_name);
	if(!file) 
	{
		throw runtime_error("File doesnot exist.\n");
	}
	else
	{
		if(remove(file_name.c_str()) == 0)
		{
			cout << "\nFile deleted from system successfully.\n";
		}
		else
		{	
			cerr << "\nfailed to delete file from system.\n";
		}
	}	
}

void file_format(const string& file_name)
{
	ifstream test_file(file_name);
	if(!test_file)
	{
		cerr << file_name << "\ndoes not exist.\n";	
	}
	else 
	{
		ofstream file(file_name, ios::trunc);
		if(file.is_open())
		{
			cout << "\nfile formatted successfully.\n";
			file.close();
		}
		else
		{
			cerr << "\nerror! file not formatted.\n";
		}
	}
}