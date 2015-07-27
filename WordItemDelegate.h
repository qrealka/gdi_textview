#ifndef _FBD9C173_68C4_43A2_B1D0_6925019E1AD1_
#define _FBD9C173_68C4_43A2_B1D0_6925019E1AD1_

#include "IItemDelegate.h"
#include "IListIndex.h"
#include <memory>
#include <locale>
	
namespace kofax
{

struct IStackLayoutView;
struct IStyleView;

class WordItemDelegate : public IItemDelegate
{
public:
	WordItemDelegate(const std::shared_ptr<IStackLayoutView>& layout, const std::locale& locale);
	void SetStyles(IStyleView* styleOne, IStyleView* styleTwo);

private:
	void SizeHint(int width, int height, IListIndex* index) final override;
	void AddLineToLayout(const std::unique_ptr<IListIndex>& textIterator, 
		const std::shared_ptr<IStackLayoutView>& layout, size_t& wordCounter);

private:
	//size_t m_wordCounter;
	std::weak_ptr<IStackLayoutView> m_layout;
	std::shared_ptr<IStyleView> m_wordStyleOne;
	std::shared_ptr<IStyleView> m_wordStyleTwo;
	std::locale  m_locale;
};

}

#endif
