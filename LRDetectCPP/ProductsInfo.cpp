#include "ProductsInfo.h"
#include "MsiProgram.h"
#include "MsiCustomProgram.h"

#include "Html.h"
#include "MsiCodes.h"

namespace ProductInfo {


void collectData(std::string& content)
{
	//1. Check if LoadRunner Full version is installed
	MsiCustomProgram lr(MsiCodes::LR, "Mercury Interactive\\LoadRunner");

	content.append(Html::h2("Installed Products Information"));
	content.append(Html::table_open());
	content.append(Html::tr(Html::td2("LoadRunner information")));

	content.append(Html::tr(Html::td("Is full LoadRunner installed?") + Html::td(lr.get_NameVersionDate())));
	content.append(Html::tr(Html::td("Patches installed") + Html::td(lr.get_InstalledPatchesString())));
	content.append(Html::tr(Html::td("Custom components installed") + Html::td(lr.get_CustomComponents())));
	content.append(Html::tr(Html::td("Main executable files") + Html::td("")));
	content.append(Html::tr(Html::td("Related environment variables") + Html::td("")));

	content.append(Html::table_close);

}

} // close namespace