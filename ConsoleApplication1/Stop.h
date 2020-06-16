//LL object for an elevator stop
#include <cstddef>
class Stop
{
public:
	int floor;
	Stop* stop_above;
	Stop* stop_below;
	bool onHold;

	Stop(int f) {
		floor = f;
		stop_above = NULL;
		stop_below = NULL;
		onHold = false;
	}
};

