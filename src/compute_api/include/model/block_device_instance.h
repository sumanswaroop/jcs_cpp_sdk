#include <string>

namespace model{

class block_device_instance{
	public:
		bool deleteOnTermination;
		string status;
		string deviceName;
		string volumeId;
};

}