#include "WordItemDelegate.h"
#include "IStackLayoutView.h"
#include "IStyleView.h"
#include "IListModel.h"
#include "WordWrapLayoutItem.h"
#include <assert.h>


namespace kofax
{

WordItemDelegate::WordItemDelegate(const std::shared_ptr<IListModel>& model,
	const std::locale& locale)
	: m_model(model)
	, m_locale(locale)
{
	assert(model);
}

void WordItemDelegate::SetStyles(IStyleView* styleOne, IStyleView* styleTwo)
{
	m_wordStyleOne.reset(styleOne);
	m_wordStyleTwo.reset(styleTwo);
}

void WordItemDelegate::AddLineToLayout(const std::unique_ptr<IListIndex>& textIterator,
	IStackLayoutView& layout, size_t& wordCounter)
{
	const auto& facet = std::use_facet<std::ctype<wchar_t> >(m_locale);
	const auto& str = textIterator->ToString();
	auto wordBegin = str.c_str();

	for (auto it = str.c_str(), end = str.c_str() + str.length();; ++it)
	{
		const bool isEOL = it >= end;
		const bool isWordSpaces = facet.is(std::ctype_base::space, *wordBegin);
		const bool isSpace = isEOL ? isWordSpaces : facet.is(std::ctype_base::space, *it);

		if (isSpace != isWordSpaces || isEOL)
		{
			if (isWordSpaces)
			{
				layout.AddSpaces(WordWrapLayoutItem::MakeWordWrappedText(layout, wordBegin, it, isEOL, m_wordStyleOne));
			}
			else
			{
				if (++wordCounter == 3)
				{
					layout.ItemPush(WordWrapLayoutItem::MakeWordWrappedText(layout, wordBegin, it, isEOL, m_wordStyleTwo));
					wordCounter = 0;
				}
				else
				{
					layout.ItemPush(WordWrapLayoutItem::MakeWordWrappedText(layout, wordBegin, it, isEOL, m_wordStyleOne));
				}
			}

			if (isEOL)
				break;

			wordBegin = it;
		}
	}
}

void WordItemDelegate::OnWindowResize(int width, int height, IStackLayoutView& layout)
{
	layout.Clear();

	if (width <= 0 || height <= 0)
		return;

	if (!m_wordStyleOne || !m_wordStyleTwo)
		return;

	if (auto model = m_model.lock())
	{
		std::unique_ptr<IListIndex> textIterator(model->GetIndex(0));

		size_t wordCounter = 0;
		while (textIterator->IsValid())
		{
			AddLineToLayout(textIterator, layout, wordCounter);
			if (!textIterator->GetNext())
				return;
		}
	}
}

}