#ifndef _2B73411A_8791_4347_A2BD_E445BFD71C07_
#define _2B73411A_8791_4347_A2BD_E445BFD71C07_

#include <functional>

namespace kofax
{

class ScopeGuard {
public:
	template<class Callable>
	ScopeGuard(Callable && undo_func) : f(std::forward<Callable>(undo_func)) {}

	ScopeGuard(ScopeGuard && other) : f(std::move(other.f)) {
		other.f = nullptr;
	}

	~ScopeGuard() {
		if (f) f(); // must not throw
	}

	void dismiss() throw() {
		f = nullptr;
	}

private:
	ScopeGuard(const ScopeGuard&) = delete;
	void operator = (const ScopeGuard&) = delete;

	std::function<void()> f;
};

}

#endif