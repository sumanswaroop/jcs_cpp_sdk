#include <vector>
#include <string>

using namespace std;

class detach_volume_request
{
private:
	string instance_id
	string volume_id;

public:

	detach_volume_request()
	{
		instance_id = "";
		volume_id = "";
	}

	string get_instance_id()
	{
		return instance_id;
	}

	void set_instance_id(string instance_id_)
	{
		instance_id = instance_id_;
	}

	string get_volume_id()
	{
		return volume_id;
	}

	void set_volume_id(string volume_id_)
	{
		volume_id = volume_id_;
	}

};