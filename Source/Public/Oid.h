#pragma once

#ifndef OID_H
#define OID_H

void SetOIDType(BYTE* buffer, int bufferLen, uint32_t& oidType)
{
	oidType = 0;
	if (*(uint8_t*)(buffer + bufferLen - 1) == 0xFF)
		oidType = 1;
	else if (*(uint32_t*)(buffer + 12) == 0)
	{
		oidType = 2;
	}
}

template <bool bBigEndian>
struct OID
{
	uint32_t oidType;
	bool bHasIDs =false;
	std::vector<std::string> nameList;
	OID(BYTE* buffer, int bufferLen, modelBone_t* joints,std::map<uint32_t,uint32_t>& globalToFinal)
	{
		SetOIDType(buffer, bufferLen, oidType);
		uint32_t offset = 0;
		switch (oidType)
		{
		case 0: //Plain strings
		{
			std::string s = std::string(reinterpret_cast<const char*>(buffer), bufferLen);

			std::string delimiter = "\r\n";
			int pos = 0;
			std::string token;
			while ((pos = s.find(delimiter)) != -1) {
				token = s.substr(0, pos);
				if(token.rfind(";", 0) && token.size())
					nameList.push_back(token);
				s.erase(0, pos + delimiter.length());
			}
			if (s.rfind(";", 0) && s.size())
				nameList.push_back(s);
			bHasIDs = true;
			break;
		}
		case 1: //Strings with lengths
		{
			uint32_t length;
			while (offset + 1 < bufferLen)
			{
				length = *(uint8_t*)(buffer + offset);
				offset += 1;
				nameList.push_back(std::string(reinterpret_cast<const char*>(buffer + offset), length));
				offset += length;
			}
			if (nameList[0] == "HeaderCharaOid")
			{
				bHasIDs = true;
				if (nameList.size() < 4)
					return;
				for (auto j = 0; j < 3; j++)
					nameList.erase(nameList.begin());
			}
		}
			break;
		case 2: //Oid files with hashed names, we skip
			return;
			break;
		default:
			break;
		}

		if (bHasIDs)
		{
			std::string delimiter = ",";
			uint32_t pos = 0;
			uint32_t id;
			for (std::string& name : nameList)
			{
				pos = name.find(delimiter);
				id = std::stoi(name.substr(0, pos));
				name.erase(0, pos + delimiter.length());
				if (globalToFinal.find(id) != globalToFinal.end())
				{
					snprintf(joints[globalToFinal[id]].name, 128, &name[0]);
				}
			}
		}
		else 
		{
			uint32_t k = 0;
			for (std::string& name : nameList)
			{
				if (k < globalToFinal.size())
				{
					snprintf(joints[k].name, 128, &name[0]);
				}
				k += 1;
			}
		}
	}
};

#endif // !OID_H
