#include <vector>
#include <string>

using namespace std;

class detach_volume_request
{
private:
	string instance_id
	string volume_id;

public:
	string get_instance_id();

	void set_instance_id(string instance_id_);

	string get_volume_id();

	void set_volume_id(string volume_id_);

};