#ifndef _FBD9C173_68C4_43A2_B1D0_6925019E1AD1_
#define _FBD9C173_68C4_43A2_B1D0_6925019E1AD1_

#include "IItemDelegate.h"
#include <memory>

namespace kofax
{

struct IStackLayoutView;

class WordItemDelegate : public IItemDelegate
{
public:
	explicit WordItemDelegate(const std::shared_ptr<IStackLayoutView>& layout);

private:
	void OnSize(int width, int height, const IListIndex& index) final override;

private:
	size_t m_wordCounter;
	std::weak_ptr<IStackLayoutView> m_layout;
};

}

#endif
