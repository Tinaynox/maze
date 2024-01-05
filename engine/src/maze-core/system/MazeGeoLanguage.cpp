//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeCoreHeader.hpp"
#include "maze-core/system/MazeGeoLanguage.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static Map<GeoLanguage, GeoLanguageData> const c_geoLanguages = 
    {
        std::make_pair(GeoLanguage::Afrikaans, GeoLanguageData{ "Afrikaans", "af", {1078} }),
        std::make_pair(GeoLanguage::Amharic, GeoLanguageData{ "Amharic", "am", {1118} }),
        std::make_pair(GeoLanguage::Arabic, GeoLanguageData{ "Arabic", "ar", {5121, 15361, 3073, 2049, 11265, 13313, 12289, 4097, 6145, 8193, 16385, 1025, 10241, 7169, 14337, 9217} }),
        std::make_pair(GeoLanguage::Mapudungun, GeoLanguageData{ "Mapudungun", "arn" }),
        std::make_pair(GeoLanguage::Assamese, GeoLanguageData{ "Assamese", "as", {1101} }),
        std::make_pair(GeoLanguage::Azerbaijani, GeoLanguageData{ "Azerbaijani", "az" }),
        std::make_pair(GeoLanguage::Bashkir, GeoLanguageData{ "Bashkir", "ba" }),
        std::make_pair(GeoLanguage::Belarusian, GeoLanguageData{ "Belarusian", "be", {1059} }),
        std::make_pair(GeoLanguage::Bulgarian, GeoLanguageData{ "Bulgarian", "bg", {1026} }),
        std::make_pair(GeoLanguage::Bengali, GeoLanguageData{ "Bengali", "bn", {2117, 1093} }),
        std::make_pair(GeoLanguage::Tibetan, GeoLanguageData{ "Tibetan", "bo", {1105} }),
        std::make_pair(GeoLanguage::Breton, GeoLanguageData{ "Breton", "br" }),
        std::make_pair(GeoLanguage::Bosnian, GeoLanguageData{ "Bosnian", "bs", {5146} }),
        std::make_pair(GeoLanguage::Catalan, GeoLanguageData{ "Catalan", "ca", {1027} }),
        std::make_pair(GeoLanguage::Corsican, GeoLanguageData{ "Corsican", "co" }),
        std::make_pair(GeoLanguage::Czech, GeoLanguageData{ "Czech", "cs", {1029} }),
        std::make_pair(GeoLanguage::Welsh, GeoLanguageData{ "Welsh", "cy", {1106} }),
        std::make_pair(GeoLanguage::Danish, GeoLanguageData{ "Danish", "da", {1030} }),
        std::make_pair(GeoLanguage::German, GeoLanguageData{ "German", "de", {3079, 1031, 5127, 4103, 2055} }),
        std::make_pair(GeoLanguage::LowerSorbian, GeoLanguageData{ "LowerSorbian", "dsb" }),
        std::make_pair(GeoLanguage::Divehi, GeoLanguageData{ "Divehi", "dv" }),
        std::make_pair(GeoLanguage::Greek, GeoLanguageData{ "Greek", "el", {1032} }),
        std::make_pair(GeoLanguage::English, GeoLanguageData{ "English", "en", {3081, 10249, 4105, 9225, 2057, 16393, 6153, 8201, 5129, 13321, 7177, 11273, 1033, 12297} }),
        std::make_pair(GeoLanguage::Spanish, GeoLanguageData{ "Spanish", "es", {11274, 16394, 13322, 9226, 5130, 7178, 12298, 17418, 4106, 18442, 2058, 19466, 6154, 15370, 10250, 20490, 1034, 14346, 8202} }),
        std::make_pair(GeoLanguage::Estonian, GeoLanguageData{ "Estonian", "et", {1061} }),
        std::make_pair(GeoLanguage::Basque, GeoLanguageData{ "Basque", "eu", {1069} }),
        std::make_pair(GeoLanguage::Persian, GeoLanguageData{ "Persian", "fa", {1065} }),
        std::make_pair(GeoLanguage::Finnish, GeoLanguageData{ "Finnish", "fi", {1035} }),
        std::make_pair(GeoLanguage::Filipino, GeoLanguageData{ "Filipino", "fil", {1124} }),
        std::make_pair(GeoLanguage::Faroese, GeoLanguageData{ "Faroese", "fo", {1080} }),
        std::make_pair(GeoLanguage::French, GeoLanguageData{ "French", "fr", {2060, 11276, 3084, 9228, 12300, 1036, 5132, 13324, 6156, 14348, 10252, 4108, 7180} }),
        std::make_pair(GeoLanguage::Frisian, GeoLanguageData{ "Frisian", "fy", {1122} }),
        std::make_pair(GeoLanguage::Irish, GeoLanguageData{ "Irish", "ga" }),
        std::make_pair(GeoLanguage::ScottishGaelic, GeoLanguageData{ "ScottishGaelic", "gd" }),
        std::make_pair(GeoLanguage::Galician, GeoLanguageData{ "Galician", "gl", {1110} }),
        std::make_pair(GeoLanguage::SwissGerman, GeoLanguageData{ "SwissGerman", "gsw" }),
        std::make_pair(GeoLanguage::Gujarati, GeoLanguageData{ "Gujarati", "gu", {1095} }),
        std::make_pair(GeoLanguage::Hausa, GeoLanguageData{ "Hausa", "ha" }),
        std::make_pair(GeoLanguage::Hebrew, GeoLanguageData{ "Hebrew", "he", {1037} }),
        std::make_pair(GeoLanguage::Hindi, GeoLanguageData{ "Hindi", "hi", {1081} }),
        std::make_pair(GeoLanguage::Croatian, GeoLanguageData{ "Croatian", "hr", {1050} }),
        std::make_pair(GeoLanguage::UpperSorbian, GeoLanguageData{ "UpperSorbian", "hsb", {1070} }),
        std::make_pair(GeoLanguage::Hungarian, GeoLanguageData{ "Hungarian", "hu", {1038} }),
        std::make_pair(GeoLanguage::Armenian, GeoLanguageData{ "Armenian", "hy", {1067} }),
        std::make_pair(GeoLanguage::Indonesian, GeoLanguageData{ "Indonesian", "id", {1057} }),
        std::make_pair(GeoLanguage::Igbo, GeoLanguageData{ "Igbo", "ig", {1136} }),
        std::make_pair(GeoLanguage::Yi, GeoLanguageData{ "Yi", "ii", {1085} }),
        std::make_pair(GeoLanguage::Icelandic, GeoLanguageData{ "Icelandic", "is", {1039} }),
        std::make_pair(GeoLanguage::Italian, GeoLanguageData{ "Italian", "it", {1040, 2064} }),
        std::make_pair(GeoLanguage::Inuktitut, GeoLanguageData{ "Inuktitut", "iu" }),
        std::make_pair(GeoLanguage::Japanese, GeoLanguageData{ "Japanese", "ja", {1041} }),
        std::make_pair(GeoLanguage::Georgian, GeoLanguageData{ "Georgian", "ka", {1079} }),
        std::make_pair(GeoLanguage::Kazakh, GeoLanguageData{ "Kazakh", "kk", {1087} }),
        std::make_pair(GeoLanguage::Greenlandic, GeoLanguageData{ "Greenlandic", "kl" }),
        std::make_pair(GeoLanguage::Khmer, GeoLanguageData{ "Khmer", "km", {1107} }),
        std::make_pair(GeoLanguage::Kannada, GeoLanguageData{ "Kannada", "kn", {1099} }),
        std::make_pair(GeoLanguage::Korean, GeoLanguageData{ "Korean", "ko", {1042} }),
        std::make_pair(GeoLanguage::Konkani, GeoLanguageData{ "Konkani", "kok", {1111} }),
        std::make_pair(GeoLanguage::Kyrgyz, GeoLanguageData{ "Kyrgyz", "ky", {1088} }),
        std::make_pair(GeoLanguage::Luxembourgish, GeoLanguageData{ "Luxembourgish", "lb" }),
        std::make_pair(GeoLanguage::Lao, GeoLanguageData{ "Lao", "lo", {1108} }),
        std::make_pair(GeoLanguage::Lithuanian, GeoLanguageData{ "Lithuanian", "lt", {1063} }),
        std::make_pair(GeoLanguage::Latvian, GeoLanguageData{ "Latvian", "lv", {1062} }),
        std::make_pair(GeoLanguage::Maori, GeoLanguageData{ "Maori", "mi", {1153} }),
        std::make_pair(GeoLanguage::Macedonian, GeoLanguageData{ "Macedonian", "mk" }),
        std::make_pair(GeoLanguage::Malayalam, GeoLanguageData{ "Malayalam", "ml", {1100} }),
        std::make_pair(GeoLanguage::Mongolian, GeoLanguageData{ "Mongolian", "mn", {2128, 1104} }),
        std::make_pair(GeoLanguage::Mohawk, GeoLanguageData{ "Mohawk", "moh" }),
        std::make_pair(GeoLanguage::Marathi, GeoLanguageData{ "Marathi", "mr", {1102} }),
        std::make_pair(GeoLanguage::Malay, GeoLanguageData{ "Malay", "ms", {2110, 1086, 1100} }),
        std::make_pair(GeoLanguage::Maltese, GeoLanguageData{ "Maltese", "mt", {1082} }),
        std::make_pair(GeoLanguage::Burmese, GeoLanguageData{ "Burmese", "my", {1109} }),
        std::make_pair(GeoLanguage::NorwegianBokmal, GeoLanguageData{ "NorwegianBokmal", "nb", {1044} }),
        std::make_pair(GeoLanguage::Nepali, GeoLanguageData{ "Nepali", "ne" }),
        std::make_pair(GeoLanguage::Dutch, GeoLanguageData{ "Dutch", "nl" }),
        std::make_pair(GeoLanguage::NorwegianNynorsk, GeoLanguageData{ "NorwegianNynorsk", "nn", {2068} }),
        std::make_pair(GeoLanguage::Norwegian, GeoLanguageData{ "Norwegian", "no" }),
        std::make_pair(GeoLanguage::Sesotho, GeoLanguageData{ "Sesotho", "st", {1072} }),
        std::make_pair(GeoLanguage::Occitan, GeoLanguageData{ "Occitan", "oc" }),
        std::make_pair(GeoLanguage::Odia, GeoLanguageData{ "Odia", "or" }),
        std::make_pair(GeoLanguage::Punjabi, GeoLanguageData{ "Punjabi", "pa", {1094} }),
        std::make_pair(GeoLanguage::Polish, GeoLanguageData{ "Polish", "pl", {1045} }),
        std::make_pair(GeoLanguage::Dari, GeoLanguageData{ "Dari", "prs" }),
        std::make_pair(GeoLanguage::Pashto, GeoLanguageData{ "Pashto", "ps" }),
        std::make_pair(GeoLanguage::Portuguese, GeoLanguageData{ "Portuguese", "pt", {1046, 2070} }),
        std::make_pair(GeoLanguage::Kiche, GeoLanguageData{ "K'iche", "quc" }),
        std::make_pair(GeoLanguage::Quechua, GeoLanguageData{ "Quechua", "qu" }),
        std::make_pair(GeoLanguage::Romansh, GeoLanguageData{ "Romansh", "rm" }),
        std::make_pair(GeoLanguage::Romanian, GeoLanguageData{ "Romanian", "ro", {2072, 1048} }),
        std::make_pair(GeoLanguage::Russian, GeoLanguageData{ "Russian", "ru", {1049, 2073} }),
        std::make_pair(GeoLanguage::Kinyarwanda, GeoLanguageData{ "Kinyarwanda", "rw" }),
        std::make_pair(GeoLanguage::Sanskrit, GeoLanguageData{ "Sanskrit", "sa", {1103} }),
        std::make_pair(GeoLanguage::Yakut, GeoLanguageData{ "Yakut", "sah" }),
        std::make_pair(GeoLanguage::SamiNorthern, GeoLanguageData{ "SamiNorthern", "se" }),
        std::make_pair(GeoLanguage::Sinhala, GeoLanguageData{ "Sinhala", "si" }),
        std::make_pair(GeoLanguage::Slovak, GeoLanguageData{ "Slovak", "sk", {1051} }),
        std::make_pair(GeoLanguage::Slovenian, GeoLanguageData{ "Slovenian", "sl", {1060} }),
        std::make_pair(GeoLanguage::SamiSouthern, GeoLanguageData{ "SamiSouthern", "sma" }),
        std::make_pair(GeoLanguage::SamiLule, GeoLanguageData{ "SamiLule", "smj" }),
        std::make_pair(GeoLanguage::SamiInari, GeoLanguageData{ "SamiInari", "smn" }),
        std::make_pair(GeoLanguage::SamiSkolt, GeoLanguageData{ "SamiSkolt", "sms" }),
        std::make_pair(GeoLanguage::Albanian, GeoLanguageData{ "Albanian", "sq", {1052} }),
        std::make_pair(GeoLanguage::Serbian, GeoLanguageData{ "Serbian", "sr", {3098, 2074} }),
        std::make_pair(GeoLanguage::Swedish, GeoLanguageData{ "Swedish", "sv", {2077, 1053} }),
        std::make_pair(GeoLanguage::Kiswahili, GeoLanguageData{ "Kiswahili", "sw" }),
        std::make_pair(GeoLanguage::Syriac, GeoLanguageData{ "Syriac", "syc", {1114} }),
        std::make_pair(GeoLanguage::Tamil, GeoLanguageData{ "Tamil", "ta", {1097} }),
        std::make_pair(GeoLanguage::Telugu, GeoLanguageData{ "Telugu", "te", {1098} }),
        std::make_pair(GeoLanguage::Tajik, GeoLanguageData{ "Tajik", "tg", {1064} }),
        std::make_pair(GeoLanguage::Thai, GeoLanguageData{ "Thai", "th", {1054} }),
        std::make_pair(GeoLanguage::Turkmen, GeoLanguageData{ "Turkmen", "tk", {1090} }),
        std::make_pair(GeoLanguage::Tswana, GeoLanguageData{ "Tswana", "tn" }),
        std::make_pair(GeoLanguage::Turkish, GeoLanguageData{ "Turkish", "tr", {1055} }),
        std::make_pair(GeoLanguage::Tatar, GeoLanguageData{ "Tatar", "tt", {1092} }),
        std::make_pair(GeoLanguage::Tamazight, GeoLanguageData{ "Tamazight", "tzm" }),
        std::make_pair(GeoLanguage::Uyghur, GeoLanguageData{ "Uyghur", "ug" }),
        std::make_pair(GeoLanguage::Ukrainian, GeoLanguageData{ "Ukrainian", "uk", {1058} }),
        std::make_pair(GeoLanguage::Urdu, GeoLanguageData{ "Urdu", "ur", {1056} }),
        std::make_pair(GeoLanguage::Uzbek, GeoLanguageData{ "Uzbek", "uz", {2115, 1091} }),
        std::make_pair(GeoLanguage::Vietnamese, GeoLanguageData{ "Vietnamese", "vi", {1066} }),
        std::make_pair(GeoLanguage::Wolof, GeoLanguageData{ "Wolof", "wo" }),
        std::make_pair(GeoLanguage::Xhosa, GeoLanguageData{ "Xhosa", "xh", {1076} }),
        std::make_pair(GeoLanguage::Yoruba, GeoLanguageData{ "Yoruba", "yo" }),
        std::make_pair(GeoLanguage::Chinese, GeoLanguageData{ "Chinese", "zh", {2052, 3076, 5124, 4100, 1028} }),
        std::make_pair(GeoLanguage::Zulu, GeoLanguageData{ "Zulu", "zu", {1077} })
    };

    //////////////////////////////////////////
    MAZE_CORE_API String const& GeoLanguageToString(GeoLanguage _location)
    {
        static String const nullString = "Unknown";

        Map<GeoLanguage, GeoLanguageData>::const_iterator it = c_geoLanguages.find(_location);
        if (it != c_geoLanguages.end())
            return it->second.name;

        return nullString;
    }
    
    //////////////////////////////////////////
    MAZE_CORE_API GeoLanguage StringToGeoLanguage(String const& _str)
    {
        for (Map<GeoLanguage, GeoLanguageData>::const_iterator  it = c_geoLanguages.begin(),
                                                                end = c_geoLanguages.end();
                                                                it != end;
                                                                ++it)
        {
            if (it->second.name == _str)
                return it->first;
        }

        return GeoLanguage::Unknown;
    }

    //////////////////////////////////////////
    MAZE_CORE_API String const& GeoLanguageToIETFTag(GeoLanguage _location)
    {
        static String const nullString = "";

        Map<GeoLanguage, GeoLanguageData>::const_iterator it = c_geoLanguages.find(_location);
        if (it != c_geoLanguages.end())
            return it->second.ietfTag;

        return nullString;
    }
    
    //////////////////////////////////////////
    MAZE_CORE_API GeoLanguage IETFTagToGeoLanguage(String const& _str)
    {
        for (Map<GeoLanguage, GeoLanguageData>::const_iterator  it = c_geoLanguages.begin(),
                                                                end = c_geoLanguages.end();
                                                                it != end;
                                                                ++it)
        {
            if (it->second.ietfTag == _str)
                return it->first;
        }

        return GeoLanguage::Unknown;
    }

    //////////////////////////////////////////
    MAZE_CORE_API GeoLanguage LocaleCodeToGeoLanguage(S32 _value)
    {
        for (Map<GeoLanguage, GeoLanguageData>::const_iterator  it = c_geoLanguages.begin(),
            end = c_geoLanguages.end();
            it != end;
            ++it)
        {
            if (it->second.localeCodes.count(_value))
                return it->first;
        }

        return GeoLanguage::Unknown;
    }


    //////////////////////////////////////////
    // Class SerializableGeoLanguage
    //
    //////////////////////////////////////////
    String SerializableGeoLanguage::toString() const
    {
        return GeoLanguageToString(m_value);
    }

    //////////////////////////////////////////
    void SerializableGeoLanguage::setString(CString _data, Size _count)
    {
        if (!_data)
        {
            m_value = GeoLanguage::Unknown;
            return;
        }

        m_value = StringToGeoLanguage(String(_data, _data + _count));
    }

} // namespace Maze
//////////////////////////////////////////
