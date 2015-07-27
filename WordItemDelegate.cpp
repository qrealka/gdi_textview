#include "WordItemDelegate.h"
#include "IStackLayoutView.h"
#include "IListIndex.h"

namespace kofax
{

WordItemDelegate::WordItemDelegate(const std::shared_ptr<IStackLayoutView>& layout)
	: m_wordCounter(0)
	, m_layout(layout)
{
}

void WordItemDelegate::OnSize(int width, int height, const IListIndex& index)
{
	if (width <= 0 || height || 0 || !index.IsValid())
		return;

	if (auto layout = m_layout.lock())
	{
		const auto& str = index.ToString();

	}
}

}