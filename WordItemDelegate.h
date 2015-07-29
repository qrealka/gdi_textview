#ifndef _FBD9C173_68C4_43A2_B1D0_6925019E1AD1_
#define _FBD9C173_68C4_43A2_B1D0_6925019E1AD1_

#include "IItemDelegate.h"
#include "IListIndex.h"
#include <memory>
#include <locale>
	
namespace kofax
{

struct AbstractStackLayout;
struct IStyleView;
struct IListModel;

class WordItemDelegate : public IItemDelegate
{
public:
	WordItemDelegate(const std::shared_ptr<IListModel>& model, const std::locale& locale);
	void SetStyles(IStyleView* styleOne, IStyleView* styleTwo);

private:
	void OnWindowResize(int width, int height, AbstractStackLayout& layout) final override;
	void AddLineToLayout(const std::unique_ptr<IListIndex>& textIterator, 
		AbstractStackLayout& layout, size_t& wordCounter);

private:
	std::weak_ptr<IListModel> m_model;
	std::shared_ptr<IStyleView> m_wordStyleOne;
	std::shared_ptr<IStyleView> m_wordStyleTwo;
	std::locale  m_locale;
};

}

#endif
