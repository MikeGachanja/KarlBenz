#include "SimpleParser.h"

bool
SimpleParser::parse(string sensors, string tag, float &value)
{
	size_t open,close;
	open = sensors.find_first_of('(');
	while (open!=string::npos)
	{		
		close = sensors.find_first_of(')',open);
		if (close != string::npos)
		{
			istringstream IN(sensors.substr(open+1,close-open-1));
			string currTag;
			IN >> currTag;
			if (currTag.compare(tag)==0)
			{
				IN >> value;
				return true;
			}
			open = sensors.find_first_of('(',close+1);
		}
		else
			return false;

	}
	return false;
}

bool
SimpleParser::parse(string sensors, string tag, int &value)
{
	size_t open,close;
	open = sensors.find_first_of('(');;
	while (open!=string::npos)
	{		
		close = sensors.find_first_of(')',open);
		if (close != string::npos)
		{
			istringstream IN(sensors.substr(open+1,close-open-1));
			string currTag;
			IN >> currTag;
			if (currTag.compare(tag)==0)
			{
				IN >> value;
				return true;
			}
			open = sensors.find_first_of('(',close+1);
		}
		else
			return false;
	}
	return false;
}

bool
SimpleParser::parse(string sensors, string tag, float *value, int size)
{
	size_t open,close;
	open = sensors.find_first_of('(');
	while (open!=string::npos)
	{		
		close = sensors.find_first_of(')',open);
		if (close != string::npos)
		{
			istringstream IN(sensors.substr(open+1,close-open-1));
			string currTag;
			IN >> currTag;
			if (currTag.compare(tag)==0)
			{				
				for (int i = 0; i < size; ++i) 
				{
					if (! (IN >> value[i]) )
						return false;
				}
				return true;
			}
			open = sensors.find_first_of('(',close+1);
		}
		else
			return false;
	}
	return false;
}

string 
SimpleParser::stringify(string tag, float value)
{
	ostringstream STR;
	STR << "(" << tag << " " << value << ")";
	return STR.str();
}

string 
SimpleParser::stringify(string tag, int value)
{
	ostringstream STR;
	STR << "(" << tag << " " << value << ")";
	return STR.str();
}

string 
SimpleParser::stringify(string tag, float *value, int size)
{
	ostringstream STR;
	STR << "(" << tag;
	for (int i = 0; i < size; ++i) 
	{
		STR << " " << value[i];
	}
	STR << ")";
	return STR.str();
}
