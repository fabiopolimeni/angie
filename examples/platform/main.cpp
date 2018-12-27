#include "angie/core/diagnostics/assert.hpp"
#include "angie/core/system/system.hpp"
#include "angie/core/system/report.hpp"
#include "angie/core/system/cpu.hpp"

#include <cstdio>
#include <cstdint>
#include <cstdlib>

void report_callback(angie::core::system::report::level lvl,
	const angie::core::types::char8* msg) {
	using namespace angie::core;

	switch (lvl) {
	case system::report::level::info:
		fprintf_s(stdout, "INFO: %s\n", msg);
		break;

	case system::report::level::debug:
		fprintf_s(stdout, "DEBUG: %s\n", msg);
		break;

	case system::report::level::performance:
		fprintf_s(stdout, "PERF: %s\n", msg);
		break;

	case system::report::level::warning:
		fprintf_s(stdout, "WARN: %s\n", msg);
		break;

	case system::report::level::error:
		fprintf_s(stderr, "ERROR: %s\n", msg);
		exit(EXIT_FAILURE);
		break;

	case system::report::level::fatal:
		fprintf_s(stderr, "FATAL: %s\n", msg);
		abort();
		break;
	}
}

int main(int32_t argc, char* argv[]) {

    using namespace angie::core;
	using namespace angie::core::system;

    if (system::init() != system::error::ok) {
        fprintf_s(stderr, "Some error occurred while initializing the system");
        exit(EXIT_FAILURE);
    }

    report::settings sets = {
        report_callback,		// report callback
        report::level::debug,	// max report level
        true,					// exit on error
        true,					// abort on fatal
        false					// callstack on exit
    };

    report::init(sets);

    containers::dynamic_array<cpu_info> cpus;
    
    system::query_cpu_info(cpus);
    report::issue(report::level::info, cpus[0].brand_name);
    system::release_cpu_info(cpus);

    system::shutdown();
    return EXIT_SUCCESS;
}