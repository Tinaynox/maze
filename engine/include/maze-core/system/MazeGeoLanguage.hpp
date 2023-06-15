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
#pragma once
#if (!defined(_MazeGeoLanguage_hpp_))
#define _MazeGeoLanguage_hpp_


//////////////////////////////////////////
#include "maze-core/MazeCoreHeader.hpp"
#include "maze-core/MazeBaseTypes.hpp"
#include "maze-core/MazeTypes.hpp"


//////////////////////////////////////////
namespace Maze
{
    struct MAZE_CORE_API GeoLanguageData
    {
        String name;
        String ietfTag;
        Set<S32> lcidCodes;
    };


    //////////////////////////////////////////
    enum class GeoLanguage
    {
        Unknown                 = 0,
        Afrikaans,
        Amharic,
        Arabic,
        Mapudungun,
        Assamese,
        Azerbaijani,
        Bashkir,
        Belarusian,
        Bulgarian,
        Bengali,
        Tibetan,
        Breton,
        Bosnian,
        Catalan,
        Corsican,
        Czech,
        Welsh,
        Danish,
        German,
        LowerSorbian,
        Divehi,
        Greek,
        English,
        Spanish,
        Estonian,
        Basque,
        Persian,
        Finnish,
        Filipino,
        Faroese,
        French,
        Frisian,
        Irish,
        ScottishGaelic,
        Galician,
        SwissGerman,
        Gujarati,
        Hausa,
        Hebrew,
        Hindi,
        Croatian,
        UpperSorbian,
        Hungarian,
        Armenian,
        Indonesian,
        Igbo,
        Yi,
        Icelandic,
        Italian,
        Inuktitut,
        Japanese,
        Georgian,
        Kazakh,
        Greenlandic,
        Khmer,
        Kannada,
        Korean,
        Konkani,
        Kyrgyz,
        Luxembourgish,
        Lao,
        Lithuanian,
        Latvian,
        Maori,
        Macedonian,
        Malayalam,
        Mongolian,
        Mohawk,
        Marathi,
        Malay,
        Maltese,
        Burmese,
        NorwegianBokmal,
        Nepali,
        Dutch,
        NorwegianNynorsk,
        Norwegian,
        Sesotho,
        Occitan,
        Odia,
        Punjabi,
        Polish,
        Dari,
        Pashto,
        Portuguese,
        Kiche,
        Quechua,
        Romansh,
        Romanian,
        Russian,
        Kinyarwanda,
        Sanskrit,
        Yakut,
        SamiNorthern,
        Sinhala,
        Slovak,
        Slovenian,
        SamiSouthern,
        SamiLule,
        SamiInari,
        SamiSkolt,
        Albanian,
        Serbian,
        Swedish,
        Kiswahili,
        Syriac,
        Tamil,
        Telugu,
        Tajik,
        Thai,
        Turkmen,
        Tswana,
        Turkish,
        Tatar,
        Tamazight,
        Uyghur,
        Ukrainian,
        Urdu,
        Uzbek,
        Vietnamese,
        Wolof,
        Xhosa,
        Yoruba,
        Chinese,
        Zulu,
           
        MAX
    };

    //////////////////////////////////////////
    String const& GeoLanguageToString(GeoLanguage _location);
    
    //////////////////////////////////////////
    GeoLanguage StringToGeoLanguage(String const& _str);


    //////////////////////////////////////////
    String const& GeoLanguageToIETFTag(GeoLanguage _location);

    //////////////////////////////////////////
    GeoLanguage IETFTagToGeoLanguage(String const& _code);


    //////////////////////////////////////////
    GeoLanguage LCIDToGeoLanguage(S32 _value);
    
} // namespace Maze
//////////////////////////////////////////
    

#endif // _MazeGeoLanguage_hpp_
//////////////////////////////////////////
