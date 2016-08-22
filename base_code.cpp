#include <xpu.h>
#include <xpu/timer.h>

using namespace xpu;

int main(int argc, char **argv) {
	int threads;
	xpu::timer t;
	
	if (argc > 1) {
		threads = atoi(argv[1]);
	} else {
		threads = 4;
	}
	
	xpu::init(threads);
}
