/*****************************************************************************
* Copyright (c) 2016 Jiocloud.com, Inc. or its affiliates.  All Rights Reserved
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish, dis-
* tribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the fol-
* lowing conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABIL-
* ITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
* SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
******************************************************************************/

#include "src/compute_api/include/model/error_response.hpp"

void error_report_parse(const string &xml_doc)
{
	XMLDocument doc;
	doc.Parse(xml_doc.c_str());

	XMLNode *RootNode = doc.FirstChild();

	XMLElement *Element = RootNode->NextSiblingElement();
	if(Element == NULL)
		return;

	for(int i = 0;i<3;i++)
	{
		if(Element != NULL )Element = Element->FirstChildElement();
		else {cout<<"Error Occured while Parsing XML or Server Didn't Respond";return;}

	}

	if(Element != NULL & Element->GetText() !=NULL)
	{
		cout<<Element->GetText()<<endl;Element=Element->NextSiblingElement();
	}
	else 
		return;

	if(Element->GetText() != NULL)
		cout<<Element->GetText();
	else
		cout<<"Error Parsing XML";

}
