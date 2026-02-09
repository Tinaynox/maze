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
#include "maze-core/system/MazeGeoLocation.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    static std::map<GeoLocation, GeoLocationData> const c_geoLocations =
    {
        std::make_pair(GeoLocation::AntiguaAndBarbuda, GeoLocationData{ "AntiguaAndBarbuda", 268, "AG", "ATG" }),
        std::make_pair(GeoLocation::Afghanistan, GeoLocationData{ "Afghanistan", 93, "AF", "AFG" }),
        std::make_pair(GeoLocation::Algeria, GeoLocationData{ "Algeria", 213, "DZ", "DZA" }),
        std::make_pair(GeoLocation::Azerbaijan, GeoLocationData{ "Azerbaijan", 994, "AZ", "AZE" }),
        std::make_pair(GeoLocation::Albania, GeoLocationData{ "Albania", 355, "AL", "ALB" }),
        std::make_pair(GeoLocation::Armenia, GeoLocationData{ "Armenia", 374, "AM", "ARM" }),
        std::make_pair(GeoLocation::Andorra, GeoLocationData{ "Andorra", 376, "AD", "AND" }),
        std::make_pair(GeoLocation::Angola, GeoLocationData{ "Angola", 244, "AO", "AGO" }),
        std::make_pair(GeoLocation::AmericanSamoa, GeoLocationData{ "AmericanSamoa", 1684, "AS", "ASM" }),
        std::make_pair(GeoLocation::Argentina, GeoLocationData{ "Argentina", 54, "AR", "ARG" }),
        std::make_pair(GeoLocation::Australia, GeoLocationData{ "Australia", 61, "AU", "AUS" }),
        std::make_pair(GeoLocation::Austria, GeoLocationData{ "Austria", 43, "AT", "AUT" }),
        std::make_pair(GeoLocation::Bahrain, GeoLocationData{ "Bahrain", 973, "BH", "BHR" }),
        std::make_pair(GeoLocation::Barbados, GeoLocationData{ "Barbados", 1246, "BB", "BRB" }),
        std::make_pair(GeoLocation::Botswana, GeoLocationData{ "Botswana", 267, "BW", "BWA" }),
        std::make_pair(GeoLocation::Bermuda, GeoLocationData{ "Bermuda", 1441, "BM", "BMU" }),
        std::make_pair(GeoLocation::Belgium, GeoLocationData{ "Belgium", 32, "BE", "BEL" }),
        std::make_pair(GeoLocation::Bahamas, GeoLocationData{ "Bahamas", 1242, "BS", "BHS" }),
        std::make_pair(GeoLocation::Bangladesh, GeoLocationData{ "Bangladesh", 880, "BD", "BGD" }),
        std::make_pair(GeoLocation::Belize, GeoLocationData{ "Belize", 501, "BZ", "BLZ" }),
        std::make_pair(GeoLocation::BosniaAndHerzegovina, GeoLocationData{ "BosniaAndHerzegovina", 387, "BA", "BIH" }),
        std::make_pair(GeoLocation::Bolivia, GeoLocationData{ "Bolivia", 591, "BO", "BOL" }),
        std::make_pair(GeoLocation::Myanmar, GeoLocationData{ "Myanmar", 95, "MM", "MMR" }),
        std::make_pair(GeoLocation::Benin, GeoLocationData{ "Benin", 229, "BJ", "BEN" }),
        std::make_pair(GeoLocation::Belarus, GeoLocationData{ "Belarus", 375, "BY", "BLR" }),
        std::make_pair(GeoLocation::SolomonIslands, GeoLocationData{ "SolomonIslands", 677, "SB", "SLB" }),
        std::make_pair(GeoLocation::Brazil, GeoLocationData{ "Brazil", 55, "BR", "BRA" }),
        std::make_pair(GeoLocation::Bhutan, GeoLocationData{ "Bhutan", 975, "BT", "BTN" }),
        std::make_pair(GeoLocation::Bulgaria, GeoLocationData{ "Bulgaria", 359, "BG", "BGR" }),
        std::make_pair(GeoLocation::Brunei, GeoLocationData{ "Brunei", 673, "BN", "BRN" }),
        std::make_pair(GeoLocation::Burundi, GeoLocationData{ "Burundi", 257, "BI", "BDI" }),
        std::make_pair(GeoLocation::Canada, GeoLocationData{ "Canada", 1, "CA", "CAN" }),
        std::make_pair(GeoLocation::Cambodia, GeoLocationData{ "Cambodia", 855, "KH", "KHM" }),
        std::make_pair(GeoLocation::Chad, GeoLocationData{ "Chad", 235, "TD", "TCD" }),
        std::make_pair(GeoLocation::SriLanka, GeoLocationData{ "SriLanka", 94, "LK", "LKA" }),
        std::make_pair(GeoLocation::Congo, GeoLocationData{ "Congo", 242, "CG", "COG" }),
        std::make_pair(GeoLocation::CongoDRC, GeoLocationData{ "CongoDRC", 242, "CG", "COG" }),
        std::make_pair(GeoLocation::China, GeoLocationData{ "China", 86, "CN", "CHN" }),
        std::make_pair(GeoLocation::Chile, GeoLocationData{ "Chile", 56, "CL", "CHL" }),
        std::make_pair(GeoLocation::Cameroon, GeoLocationData{ "Cameroon", 237, "CM", "CMR" }),
        std::make_pair(GeoLocation::Comoros, GeoLocationData{ "Comoros", 269, "KM", "COM" }),
        std::make_pair(GeoLocation::Colombia, GeoLocationData{ "Colombia", 57, "CO", "COL" }),
        std::make_pair(GeoLocation::CostaRica, GeoLocationData{ "CostaRica", 506, "CR", "CRI" }),
        std::make_pair(GeoLocation::CentralAfricanRepublic, GeoLocationData{ "CentralAfricanRepublic", 236, "CF", "CAF" }),
        std::make_pair(GeoLocation::Cuba, GeoLocationData{ "Cuba", 53, "CU", "CUB" }),
        std::make_pair(GeoLocation::CaboVerde, GeoLocationData{ "CaboVerde", 238, "CV", "CPV" }),
        std::make_pair(GeoLocation::Cyprus, GeoLocationData{ "Cyprus", 357, "CY", "CYP" }),
        std::make_pair(GeoLocation::Denmark, GeoLocationData{ "Denmark", 45, "DK", "DNK" }),
        std::make_pair(GeoLocation::Djibouti, GeoLocationData{ "Djibouti", 253, "DJ", "DJI" }),
        std::make_pair(GeoLocation::Dominica, GeoLocationData{ "Dominica", 767, "DM", "DMA" }),
        std::make_pair(GeoLocation::DominicanRepublic, GeoLocationData{ "DominicanRepublic", 1809, "DO", "DOM" }),
        std::make_pair(GeoLocation::Ecuador, GeoLocationData{ "Ecuador", 593, "EC", "ECU" }),
        std::make_pair(GeoLocation::Egypt, GeoLocationData{ "Egypt", 20, "EG", "EGY" }),
        std::make_pair(GeoLocation::Ireland, GeoLocationData{ "Ireland", 353, "IE", "IRL" }),
        std::make_pair(GeoLocation::EquatorialGuinea, GeoLocationData{ "EquatorialGuinea", 240, "GQ", "GNQ" }),
        std::make_pair(GeoLocation::Estonia, GeoLocationData{ "Estonia", 372, "EE", "EST" }),
        std::make_pair(GeoLocation::Eritrea, GeoLocationData{ "Eritrea", 291, "ER", "ERI" }),
        std::make_pair(GeoLocation::ElSalvador, GeoLocationData{ "ElSalvador", 503, "SV", "SLV" }),
        std::make_pair(GeoLocation::Ethiopia, GeoLocationData{ "Ethiopia", 251, "ET", "ETH" }),
        std::make_pair(GeoLocation::CzechRepublic, GeoLocationData{ "CzechRepublic", 420, "CZ", "CZE" }),
        std::make_pair(GeoLocation::Finland, GeoLocationData{ "Finland", 358, "FI", "FIN" }),
        std::make_pair(GeoLocation::Fiji, GeoLocationData{ "Fiji", 679, "FJ", "FJI" }),
        std::make_pair(GeoLocation::Micronesia1, GeoLocationData{ "Micronesia1", 691, "FM", "FSM" }),
        std::make_pair(GeoLocation::FaroeIslands, GeoLocationData{ "FaroeIslands", 298, "FO", "FRO" }),
        std::make_pair(GeoLocation::France, GeoLocationData{ "France", 33, "FR", "FRA" }),
        std::make_pair(GeoLocation::Gambia, GeoLocationData{ "Gambia", 220, "GM", "GMB" }),
        std::make_pair(GeoLocation::Gabon, GeoLocationData{ "Gabon", 241, "GA", "GAB" }),
        std::make_pair(GeoLocation::Georgia, GeoLocationData{ "Georgia", 995, "GE", "GEO" }),
        std::make_pair(GeoLocation::Ghana, GeoLocationData{ "Ghana", 233, "GH", "GHA" }),
        std::make_pair(GeoLocation::Gibraltar, GeoLocationData{ "Gibraltar", 350, "GI", "GIB" }),
        std::make_pair(GeoLocation::Grenada, GeoLocationData{ "Grenada", 1473, "GD", "GRD" }),
        std::make_pair(GeoLocation::Greenland, GeoLocationData{ "Greenland", 299, "GL", "GRL" }),
        std::make_pair(GeoLocation::Germany, GeoLocationData{ "Germany", 49, "DE", "DEU" }),
        std::make_pair(GeoLocation::Greece, GeoLocationData{ "Greece", 30, "GR", "GRC" }),
        std::make_pair(GeoLocation::Guatemala, GeoLocationData{ "Guatemala", 502, "GT", "GTM" }),
        std::make_pair(GeoLocation::Guinea, GeoLocationData{ "Guinea", 224, "GN", "GIN" }),
        std::make_pair(GeoLocation::Guyana, GeoLocationData{ "Guyana", 592, "GY", "GUY" }),
        std::make_pair(GeoLocation::Haiti, GeoLocationData{ "Haiti", 509, "HT", "HTI" }),
        std::make_pair(GeoLocation::HongKongSAR, GeoLocationData{ "HongKongSAR", 852, "HK", "HKG" }),
        std::make_pair(GeoLocation::Honduras, GeoLocationData{ "Honduras", 504, "HN", "HND" }),
        std::make_pair(GeoLocation::Croatia, GeoLocationData{ "Croatia", 385, "HR", "HRV" }),
        std::make_pair(GeoLocation::Hungary, GeoLocationData{ "Hungary", 36, "HU", "HUN" }),
        std::make_pair(GeoLocation::Iceland, GeoLocationData{ "Iceland", 354, "IS", "ISL" }),
        std::make_pair(GeoLocation::Indonesia, GeoLocationData{ "Indonesia", 62, "ID", "IDN" }),
        std::make_pair(GeoLocation::India, GeoLocationData{ "India", 91, "IN", "IND" }),
        std::make_pair(GeoLocation::BritishIndianOceanTerritory, GeoLocationData{ "BritishIndianOceanTerritory", 246, "IO", "IOT" }),
        std::make_pair(GeoLocation::Iran, GeoLocationData{ "Iran", 98, "IR", "IRN" }),
        std::make_pair(GeoLocation::Israel, GeoLocationData{ "Israel", 972, "IL", "ISR" }),
        std::make_pair(GeoLocation::Italy, GeoLocationData{ "Italy", 39, "IT", "ITA" }),
        std::make_pair(GeoLocation::CotedIvoire, GeoLocationData{ "CotedIvoire", 225, "CI", "CIV" }),
        std::make_pair(GeoLocation::Iraq, GeoLocationData{ "Iraq", 964, "IQ", "IRQ" }),
        std::make_pair(GeoLocation::Japan, GeoLocationData{ "Japan", 81, "JP", "JPN" }),
        std::make_pair(GeoLocation::Jamaica, GeoLocationData{ "Jamaica", 1876, "JM", "JAM" }),
        std::make_pair(GeoLocation::JanMayen, GeoLocationData{ "JanMayen", 47, "SJ", "SJM" }),
        std::make_pair(GeoLocation::Jordan, GeoLocationData{ "Jordan", 962, "JO", "JOR" }),
        std::make_pair(GeoLocation::JohnstonAtoll, GeoLocationData{ "JohnstonAtoll", -1, "", "" }),
        std::make_pair(GeoLocation::Kenya, GeoLocationData{ "Kenya", 254, "KE", "KEN" }),
        std::make_pair(GeoLocation::Kyrgyzstan, GeoLocationData{ "Kyrgyzstan", 996, "KG", "KGZ" }),
        std::make_pair(GeoLocation::NorthKorea, GeoLocationData{ "NorthKorea", 850, "KP", "PRK" }),
        std::make_pair(GeoLocation::Kiribati, GeoLocationData{ "Kiribati", 686, "KI", "KIR" }),
        std::make_pair(GeoLocation::Korea, GeoLocationData{ "Korea", 82, "KR", "KOR" }),
        std::make_pair(GeoLocation::Kuwait, GeoLocationData{ "Kuwait", 965, "KW", "KWT" }),
        std::make_pair(GeoLocation::Kazakhstan, GeoLocationData{ "Kazakhstan", 7, "KZ", "KAZ" }),
        std::make_pair(GeoLocation::Laos, GeoLocationData{ "Laos", 856, "LA", "LAO" }),
        std::make_pair(GeoLocation::Lebanon, GeoLocationData{ "Lebanon", 961, "LB", "LBN" }),
        std::make_pair(GeoLocation::Latvia, GeoLocationData{ "Latvia", 371, "LV", "LVA" }),
        std::make_pair(GeoLocation::Lithuania, GeoLocationData{ "Lithuania", 370, "LT", "LTU" }),
        std::make_pair(GeoLocation::Liberia, GeoLocationData{ "Liberia", 231, "LR", "LBR" }),
        std::make_pair(GeoLocation::Slovakia, GeoLocationData{ "Slovakia", 421, "SK", "SVK" }),
        std::make_pair(GeoLocation::Liechtenstein, GeoLocationData{ "Liechtenstein", 423, "LI", "LIE" }),
        std::make_pair(GeoLocation::Lesotho, GeoLocationData{ "Lesotho", 266, "LS", "LSO" }),
        std::make_pair(GeoLocation::Luxembourg, GeoLocationData{ "Luxembourg", 352, "LU", "LUX" }),
        std::make_pair(GeoLocation::Libya, GeoLocationData{ "Libya", 218, "LY", "LBY" }),
        std::make_pair(GeoLocation::Madagascar, GeoLocationData{ "Madagascar", 261, "MG", "MDG" }),
        std::make_pair(GeoLocation::MacaoSAR, GeoLocationData{ "MacaoSAR", 853, "MO", "MAC" }),
        std::make_pair(GeoLocation::Moldova, GeoLocationData{ "Moldova", 373, "MD", "MDA" }),
        std::make_pair(GeoLocation::Mongolia, GeoLocationData{ "Mongolia", 976, "MN", "MNG" }),
        std::make_pair(GeoLocation::Malawi, GeoLocationData{ "Malawi", 265, "MW", "MWI" }),
        std::make_pair(GeoLocation::Mali, GeoLocationData{ "Mali", 223, "ML", "MLI" }),
        std::make_pair(GeoLocation::Monaco, GeoLocationData{ "Monaco", 377, "MC", "MCO" }),
        std::make_pair(GeoLocation::Morocco, GeoLocationData{ "Morocco", 212, "MA", "MAR" }),
        std::make_pair(GeoLocation::Mauritius, GeoLocationData{ "Mauritius", 230, "MU", "MUS" }),
        std::make_pair(GeoLocation::Mauritania, GeoLocationData{ "Mauritania", 222, "MR", "MRT" }),
        std::make_pair(GeoLocation::Malta, GeoLocationData{ "Malta", 356, "MT", "MLT" }),
        std::make_pair(GeoLocation::Oman, GeoLocationData{ "Oman", 968, "OM", "OMN" }),
        std::make_pair(GeoLocation::Maldives, GeoLocationData{ "Maldives", 960, "MV", "MDV" }),
        std::make_pair(GeoLocation::Mexico, GeoLocationData{ "Mexico", 52, "MX", "MEX" }),
        std::make_pair(GeoLocation::Malaysia, GeoLocationData{ "Malaysia", 60, "MY", "MYS" }),
        std::make_pair(GeoLocation::Mozambique, GeoLocationData{ "Mozambique", 258, "MZ", "MOZ" }),
        std::make_pair(GeoLocation::Niger, GeoLocationData{ "Niger", 227, "NE", "NER" }),
        std::make_pair(GeoLocation::Vanuatu, GeoLocationData{ "Vanuatu", 678, "VU", "VUT" }),
        std::make_pair(GeoLocation::Nigeria, GeoLocationData{ "Nigeria", 234, "NG", "NGA" }),
        std::make_pair(GeoLocation::Netherlands, GeoLocationData{ "Netherlands", 31, "NL", "NLD" }),
        std::make_pair(GeoLocation::Norway, GeoLocationData{ "Norway", 47, "NO", "NOR" }),
        std::make_pair(GeoLocation::Nepal, GeoLocationData{ "Nepal", 977, "NP", "NPL" }),
        std::make_pair(GeoLocation::Nauru, GeoLocationData{ "Nauru", 674, "NR", "NRU" }),
        std::make_pair(GeoLocation::Suriname, GeoLocationData{ "Suriname", 597, "SR", "SUR" }),
        std::make_pair(GeoLocation::Nicaragua, GeoLocationData{ "Nicaragua", 505, "NI", "NIC" }),
        std::make_pair(GeoLocation::NewZealand, GeoLocationData{ "NewZealand", 64, "NZ", "NZL" }),
        std::make_pair(GeoLocation::PalestinianAuthority, GeoLocationData{ "PalestinianAuthority", 970, "PS", "PSE" }),
        std::make_pair(GeoLocation::Paraguay, GeoLocationData{ "Paraguay", 595, "PY", "PRY" }),
        std::make_pair(GeoLocation::Peru, GeoLocationData{ "Peru", 51, "PE", "PER" }),
        std::make_pair(GeoLocation::Pakistan, GeoLocationData{ "Pakistan", 92, "PK", "PAK" }),
        std::make_pair(GeoLocation::Poland, GeoLocationData{ "Poland", 48, "PL", "POL" }),
        std::make_pair(GeoLocation::Panama, GeoLocationData{ "Panama", 507, "PA", "PAN" }),
        std::make_pair(GeoLocation::Portugal, GeoLocationData{ "Portugal", 351, "PT", "PRT" }),
        std::make_pair(GeoLocation::PapuaNewGuinea, GeoLocationData{ "PapuaNewGuinea", 675, "PG", "PNG" }),
        std::make_pair(GeoLocation::Palau, GeoLocationData{ "Palau", 680, "PW", "PLW" }),
        std::make_pair(GeoLocation::GuineaBissau, GeoLocationData{ "GuineaBissau", 245, "GW", "GNB" }),
        std::make_pair(GeoLocation::Qatar, GeoLocationData{ "Qatar", 974, "QA", "QAT" }),
        std::make_pair(GeoLocation::Reunion, GeoLocationData{ "Reunion", 262, "RE", "REU" }),
        std::make_pair(GeoLocation::MarshallIslands, GeoLocationData{ "MarshallIslands", 692, "MH", "MHL" }),
        std::make_pair(GeoLocation::Romania, GeoLocationData{ "Romania", 40, "RO", "ROU" }),
        std::make_pair(GeoLocation::Philippines, GeoLocationData{ "Philippines", 63, "PH", "PHL" }),
        std::make_pair(GeoLocation::PuertoRico, GeoLocationData{ "PuertoRico", 1787, "PR", "PRI" }),
        std::make_pair(GeoLocation::Russia, GeoLocationData{ "Russia", 7, "RU", "RUS" }),
        std::make_pair(GeoLocation::Rwanda, GeoLocationData{ "Rwanda", 250, "RW", "RWA" }),
        std::make_pair(GeoLocation::SaudiArabia, GeoLocationData{ "SaudiArabia", 966, "SA", "SAU" }),
        std::make_pair(GeoLocation::SaintPierreAndMiquelon, GeoLocationData{ "SaintPierreAndMiquelon", 508, "PM", "SPM" }),
        std::make_pair(GeoLocation::SaintKittsAndNevis, GeoLocationData{ "SaintKittsAndNevis", 1869, "KN", "KNA" }),
        std::make_pair(GeoLocation::Seychelles, GeoLocationData{ "Seychelles", 248, "SC", "SYC" }),
        std::make_pair(GeoLocation::SouthAfrica, GeoLocationData{ "SouthAfrica", 27, "ZA", "ZAF" }),
        std::make_pair(GeoLocation::Senegal, GeoLocationData{ "Senegal", 221, "SN", "SEN" }),
        std::make_pair(GeoLocation::Slovenia, GeoLocationData{ "Slovenia", 386, "SI", "SVN" }),
        std::make_pair(GeoLocation::SierraLeone, GeoLocationData{ "SierraLeone", 232, "SL", "SLE" }),
        std::make_pair(GeoLocation::SanMarino, GeoLocationData{ "SanMarino", 378, "SM", "SMR" }),
        std::make_pair(GeoLocation::Singapore, GeoLocationData{ "Singapore", 65, "SG", "SGP" }),
        std::make_pair(GeoLocation::Somalia, GeoLocationData{ "Somalia", 252, "SO", "SOM" }),
        std::make_pair(GeoLocation::Spain, GeoLocationData{ "Spain", 34, "ES", "ESP" }),
        std::make_pair(GeoLocation::SaintLucia, GeoLocationData{ "SaintLucia", 1758, "LC", "LCA" }),
        std::make_pair(GeoLocation::Sudan, GeoLocationData{ "Sudan", 249, "SD", "SDN" }),
        std::make_pair(GeoLocation::Svalbard, GeoLocationData{ "Svalbard", 47, "SJ", "SJM" }),
        std::make_pair(GeoLocation::Sweden, GeoLocationData{ "Sweden", 46, "SE", "SWE" }),
        std::make_pair(GeoLocation::Syria, GeoLocationData{ "Syria", 963, "SY", "SYR" }),
        std::make_pair(GeoLocation::Switzerland, GeoLocationData{ "Switzerland", 41, "CH", "CHE" }),
        std::make_pair(GeoLocation::UnitedArabEmirates, GeoLocationData{ "UnitedArabEmirates", 971, "AE", "ARE" }),
        std::make_pair(GeoLocation::TrinidadAndTobago, GeoLocationData{ "TrinidadAndTobago", 1868, "TT", "TTO" }),
        std::make_pair(GeoLocation::Thailand, GeoLocationData{ "Thailand", 66, "TH", "THA" }),
        std::make_pair(GeoLocation::Tajikistan, GeoLocationData{ "Tajikistan", 992, "TJ", "TJK" }),
        std::make_pair(GeoLocation::Tonga, GeoLocationData{ "Tonga", 676, "TO", "TON" }),
        std::make_pair(GeoLocation::Togo, GeoLocationData{ "Togo", 228, "TG", "TGO" }),
        std::make_pair(GeoLocation::SaoTomeAndPrincipe, GeoLocationData{ "SaoTomeAndPrincipe", 239, "ST", "STP" }),
        std::make_pair(GeoLocation::Tunisia, GeoLocationData{ "Tunisia", 216, "TN", "TUN" }),
        std::make_pair(GeoLocation::Turkey, GeoLocationData{ "Turkey", 90, "TR", "TUR" }),
        std::make_pair(GeoLocation::Tuvalu, GeoLocationData{ "Tuvalu", 688, "TV", "TUV" }),
        std::make_pair(GeoLocation::Taiwan, GeoLocationData{ "Taiwan", 886, "TW", "TWN" }),
        std::make_pair(GeoLocation::Turkmenistan, GeoLocationData{ "Turkmenistan", 993, "TM", "TKM" }),
        std::make_pair(GeoLocation::Tanzania, GeoLocationData{ "Tanzania", 255, "TZ", "TZA" }),
        std::make_pair(GeoLocation::Uganda, GeoLocationData{ "Uganda", 256, "UG", "UGA" }),
        std::make_pair(GeoLocation::Ukraine, GeoLocationData{ "Ukraine", 380, "UA", "UKR" }),
        std::make_pair(GeoLocation::UnitedKingdom, GeoLocationData{ "UnitedKingdom", 44, "GB", "GBR" }),
        std::make_pair(GeoLocation::UnitedStates, GeoLocationData{ "UnitedStates", 1, "US", "USA" }),
        std::make_pair(GeoLocation::BurkinaFaso, GeoLocationData{ "BurkinaFaso", 226, "BF", "BFA" }),
        std::make_pair(GeoLocation::Uruguay, GeoLocationData{ "Uruguay", 598, "UY", "URY" }),
        std::make_pair(GeoLocation::Uzbekistan, GeoLocationData{ "Uzbekistan", 998, "UZ", "UZB" }),
        std::make_pair(GeoLocation::SaintVincentAndtheGrenadines, GeoLocationData{ "SaintVincentAndtheGrenadines", 1784, "VC", "VCT" }),
        std::make_pair(GeoLocation::Venezuela, GeoLocationData{ "Venezuela", 58, "VE", "VEN" }),
        std::make_pair(GeoLocation::Vietnam, GeoLocationData{ "Vietnam", 84, "VN", "VNM" }),
        std::make_pair(GeoLocation::USVirginIslands, GeoLocationData{ "USVirginIslands", 1340, "VI", "VIR" }),
        std::make_pair(GeoLocation::VaticanCity, GeoLocationData{ "VaticanCity", 379, "VA", "VAT" }),
        std::make_pair(GeoLocation::Namibia, GeoLocationData{ "Namibia", 264, "NA", "NAM" }),
        std::make_pair(GeoLocation::WakeIsland, GeoLocationData{ "WakeIsland", -1, "", "" }),
        std::make_pair(GeoLocation::Samoa, GeoLocationData{ "Samoa", 685, "WS", "WSM" }),
        std::make_pair(GeoLocation::Swaziland, GeoLocationData{ "Swaziland", 268, "SZ", "SWZ" }),
        std::make_pair(GeoLocation::Yemen, GeoLocationData{ "Yemen", 967, "YE", "YEM" }),
        std::make_pair(GeoLocation::Zambia, GeoLocationData{ "Zambia", 260, "ZM", "ZMB" }),
        std::make_pair(GeoLocation::Zimbabwe, GeoLocationData{ "Zimbabwe", 263, "ZW", "ZWE" }),
        std::make_pair(GeoLocation::SerbiaAndMontenegroFormer, GeoLocationData{ "SerbiaAndMontenegroFormer", -1, "", "" }),
        std::make_pair(GeoLocation::Montenegro, GeoLocationData{ "Montenegro", 382, "ME", "MNE" }),
        std::make_pair(GeoLocation::Serbia, GeoLocationData{ "Serbia", 381, "RS", "SRB" }),
        std::make_pair(GeoLocation::Curacao, GeoLocationData{ "Curacao", 599, "CW", "CUW" }),
        std::make_pair(GeoLocation::Anguilla, GeoLocationData{ "Anguilla", 1264, "AI", "AIA" }),
        std::make_pair(GeoLocation::SouthSudan, GeoLocationData{ "SouthSudan", 211, "SS", "SSD" }),
        std::make_pair(GeoLocation::Antarctica, GeoLocationData{ "Antarctica", 672, "AQ", "ATA" }),
        std::make_pair(GeoLocation::Aruba, GeoLocationData{ "Aruba", 297, "AW", "ABW" }),
        std::make_pair(GeoLocation::AscensionIsland, GeoLocationData{ "AscensionIsland", 247, "", "" }),
        std::make_pair(GeoLocation::AshmoreAndCartierIslands, GeoLocationData{ "AshmoreAndCartierIslands", -1, "", "" }),
        std::make_pair(GeoLocation::BakerIsland, GeoLocationData{ "BakerIsland", -1, "", "" }),
        std::make_pair(GeoLocation::BouvetIsland, GeoLocationData{ "BouvetIsland", -1, "", "" }),
        std::make_pair(GeoLocation::CaymanIslands, GeoLocationData{ "CaymanIslands", 1345, "KY", "CYM" }),
        std::make_pair(GeoLocation::ChannelIslands, GeoLocationData{ "ChannelIslands", -1, "", "" }),
        std::make_pair(GeoLocation::ChristmasIsland, GeoLocationData{ "ChristmasIsland", 61, "CX", "CXR" }),
        std::make_pair(GeoLocation::ClippertonIsland, GeoLocationData{ "ClippertonIsland", -1, "", "" }),
        std::make_pair(GeoLocation::CocosKeelingIslands, GeoLocationData{ "CocosKeelingIslands",     61, "", "" }),
        std::make_pair(GeoLocation::CookIslands, GeoLocationData{ "CookIslands",     61, "CC", "CCK" }),
        std::make_pair(GeoLocation::CoralSeaIslands, GeoLocationData{ "CoralSeaIslands", -1, "", "" }),
        std::make_pair(GeoLocation::DiegoGarcia, GeoLocationData{ "DiegoGarcia", -1, "", "" }),
        std::make_pair(GeoLocation::FalklandIslands, GeoLocationData{ "FalklandIslands", 500, "FK", "FLK" }),
        std::make_pair(GeoLocation::FrenchGuiana, GeoLocationData{ "FrenchGuiana", 594, "", "" }),
        std::make_pair(GeoLocation::FrenchPolynesia, GeoLocationData{ "FrenchPolynesia", 689, "PF", "PYF" }),
        std::make_pair(GeoLocation::FrenchSouthernTerritories, GeoLocationData{ "FrenchSouthernTerritories", -1, "", "" }),
        std::make_pair(GeoLocation::Guadeloupe, GeoLocationData{ "Guadeloupe", 590, "", "" }),
        std::make_pair(GeoLocation::Guam, GeoLocationData{ "Guam", 1671, "GU", "GUM" }),
        std::make_pair(GeoLocation::GuantanamoBay, GeoLocationData{ "GuantanamoBay", 99, "", "" }),
        std::make_pair(GeoLocation::Guernsey, GeoLocationData{ "Guernsey", 441481, "GG", "GGY" }),
        std::make_pair(GeoLocation::HeardIslandAndMcDonaldIslands, GeoLocationData{ "HeardIslandAndMcDonaldIslands", -1, "", "" }),
        std::make_pair(GeoLocation::HowlandIsland, GeoLocationData{ "HowlandIsland", -1, "", "" }),
        std::make_pair(GeoLocation::JarvisIsland, GeoLocationData{ "JarvisIsland", -1, "", "" }),
        std::make_pair(GeoLocation::Jersey, GeoLocationData{ "Jersey", 441534, "JE", "JEY" }),
        std::make_pair(GeoLocation::KingmanReef, GeoLocationData{ "KingmanReef", -1, "", "" }),
        std::make_pair(GeoLocation::Martinique, GeoLocationData{ "Martinique", 596, "", "" }),
        std::make_pair(GeoLocation::Mayotte, GeoLocationData{ "Mayotte", 262, "YT", "MYT" }),
        std::make_pair(GeoLocation::Montserrat, GeoLocationData{ "Montserrat", 1664, "MS", "MSR" }),
        std::make_pair(GeoLocation::NetherlandsAntillesFormer, GeoLocationData{ "NetherlandsAntillesFormer", 599, "AN", "ANT" }),
        std::make_pair(GeoLocation::NewCaledonia, GeoLocationData{ "NewCaledonia", 687, "NC", "NCL" }),
        std::make_pair(GeoLocation::Niue, GeoLocationData{ "Niue", 683, "NU", "NIU" }),
        std::make_pair(GeoLocation::NorfolkIsland, GeoLocationData{ "NorfolkIsland", -1, "", "" }),
        std::make_pair(GeoLocation::NorthernMarianaIslands, GeoLocationData{ "NorthernMarianaIslands", 1670, "MP", "MNP" }),
        std::make_pair(GeoLocation::PalmyraAtoll, GeoLocationData{ "PalmyraAtoll", -1, "", "" }),
        std::make_pair(GeoLocation::PitcairnIslands, GeoLocationData{ "PitcairnIslands",     64, "PN", "PCN" }),
        std::make_pair(GeoLocation::RotaIsland, GeoLocationData{ "RotaIsland", -1, "", "" }),
        std::make_pair(GeoLocation::Saipan, GeoLocationData{ "Saipan", -1, "", "" }),
        std::make_pair(GeoLocation::SouthGeorgiaAndtheSouthSandwichIslands, GeoLocationData{ "SouthGeorgiaAndtheSouthSandwichIslands", -1, "", "" }),
        std::make_pair(GeoLocation::StHelenaAscensionAndTristandaCunha, GeoLocationData{ "StHelenaAscensionAndTristandaCunha", -1, "", "" }),
        std::make_pair(GeoLocation::TinianIsland, GeoLocationData{ "TinianIsland", -1, "", "" }),
        std::make_pair(GeoLocation::Tokelau, GeoLocationData{ "Tokelau",     690, "TK", "TKL" }),
        std::make_pair(GeoLocation::TristandaCunha, GeoLocationData{ "TristandaCunha", 590, "", "" }),
        std::make_pair(GeoLocation::TurksAndCaicosIslands, GeoLocationData{ "TurksAndCaicosIslands", 1649, "TC", "TCA" }),
        std::make_pair(GeoLocation::BritishVirginIslands, GeoLocationData{ "BritishVirginIslands", 1284, "VG", "VGB" }),
        std::make_pair(GeoLocation::WallisAndFutuna, GeoLocationData{ "WallisAndFutuna", 681, "WF", "WLF" }),

        std::make_pair(GeoLocation::Africa, GeoLocationData{ "Africa", -1, "", "" }),
        std::make_pair(GeoLocation::Asia, GeoLocationData{ "Asia", -1, "", "" }),
        std::make_pair(GeoLocation::Europe, GeoLocationData{ "Europe", -1, "", "" }),
        std::make_pair(GeoLocation::IsleofMan, GeoLocationData{ "IsleofMan", -1, "", "" }),
        std::make_pair(GeoLocation::MacedoniaFYRO, GeoLocationData{ "MacedoniaFYRO",     389, "", "" }),
        std::make_pair(GeoLocation::Melanesia, GeoLocationData{ "Melanesia", -1, "", "" }),
        std::make_pair(GeoLocation::Micronesia2, GeoLocationData{ "Micronesia2", 691, "", "" }),
        std::make_pair(GeoLocation::MidwayIslands, GeoLocationData{ "MidwayIslands", -1, "", "" }),
        std::make_pair(GeoLocation::NorthernAmerica, GeoLocationData{ "NorthernAmerica", -1, "", "" }),
        std::make_pair(GeoLocation::Polynesia, GeoLocationData{ "Polynesia", -1, "", "" }),
        std::make_pair(GeoLocation::CentralAmerica, GeoLocationData{ "CentralAmerica", -1, "", "" }),
        std::make_pair(GeoLocation::Oceania, GeoLocationData{ "Oceania", -1, "", "" }),
        std::make_pair(GeoLocation::SintMaarten, GeoLocationData{ "SintMaarten", -1, "", "" }),
        std::make_pair(GeoLocation::SouthAmerica, GeoLocationData{ "SouthAmerica", -1, "", "" }),
        std::make_pair(GeoLocation::SaintMartin, GeoLocationData{ "SaintMartin", -1, "", "" }),
        std::make_pair(GeoLocation::World, GeoLocationData{ "World", -1, "", "" }),
        std::make_pair(GeoLocation::WesternAfrica, GeoLocationData{ "WesternAfrica", -1, "", "" }),
        std::make_pair(GeoLocation::MiddleAfrica, GeoLocationData{ "MiddleAfrica", -1, "", "" }),
        std::make_pair(GeoLocation::NorthernAfrica, GeoLocationData{ "NorthernAfrica", -1, "", "" }),
        std::make_pair(GeoLocation::CentralAsia, GeoLocationData{ "CentralAsia", -1, "", "" }),
        std::make_pair(GeoLocation::SouthEasternAsia, GeoLocationData{ "SouthEasternAsia", -1, "", "" }),
        std::make_pair(GeoLocation::EasternAsia, GeoLocationData{ "EasternAsia", -1, "", "" }),
        std::make_pair(GeoLocation::EasternAfrica, GeoLocationData{ "EasternAfrica", -1, "", "" }),
        std::make_pair(GeoLocation::EasternEurope, GeoLocationData{ "EasternEurope", -1, "", "" }),
        std::make_pair(GeoLocation::SouthernEurope, GeoLocationData{ "SouthernEurope", -1, "", "" }),
        std::make_pair(GeoLocation::MiddleEast, GeoLocationData{ "MiddleEast", -1, "", "" }),
        std::make_pair(GeoLocation::SouthernAsia, GeoLocationData{ "SouthernAsia", -1, "", "" }),
        std::make_pair(GeoLocation::TimorLeste, GeoLocationData{ "TimorLeste", -1, "", "" }),
        std::make_pair(GeoLocation::Kosovo, GeoLocationData{ "Kosovo", 383, "", "" }),
        std::make_pair(GeoLocation::Americas, GeoLocationData{ "Americas", -1, "", "" }),
        std::make_pair(GeoLocation::AlandIslands, GeoLocationData{ "AlandIslands", -1, "", "" }),
        std::make_pair(GeoLocation::Caribbean, GeoLocationData{ "Caribbean", -1, "", "" }),
        std::make_pair(GeoLocation::NorthernEurope, GeoLocationData{ "NorthernEurope", -1, "", "" }),
        std::make_pair(GeoLocation::SouthernAfrica, GeoLocationData{ "SouthernAfrica", -1, "", "" }),
        std::make_pair(GeoLocation::WesternEurope, GeoLocationData{ "WesternEurope", -1, "", "" }),
        std::make_pair(GeoLocation::AustraliaandNewZealand, GeoLocationData{ "AustraliaandNewZealand", -1, "", "" }),
        std::make_pair(GeoLocation::SaintBarthelemy, GeoLocationData{ "SaintBarthelemy", 590, "BL", "BLM" }),
        std::make_pair(GeoLocation::USMinorOutlyingIslands, GeoLocationData{ "USMinorOutlyingIslands", -1, "", "" }),
        std::make_pair(GeoLocation::LatinAmericaandtheCaribbean, GeoLocationData{ "LatinAmericaandtheCaribbean", -1, "", "" }),
        std::make_pair(GeoLocation::BonaireSintEustatiusandSaba, GeoLocationData{ "BonaireSintEustatiusandSaba", -1, "", "" }),
    };

    //////////////////////////////////////////
    MAZE_CORE_API CString GeoLocationToString(GeoLocation _location)
    {
        std::map<GeoLocation, GeoLocationData>::const_iterator it = c_geoLocations.find(_location);
        if (it != c_geoLocations.end())
            return it->second.name;

        return "Unknown";
    }
    
    //////////////////////////////////////////
    MAZE_CORE_API GeoLocation StringToGeoLocation(CString _str)
    {
        for (std::map<GeoLocation, GeoLocationData>::const_iterator  it = c_geoLocations.begin(),
                                                                end = c_geoLocations.end();
                                                                it != end;
                                                                ++it)
        {
            if (strcmp(it->second.name, _str) == 0)
                return it->first;
        }

        return GeoLocation::Unknown;
    }

    //////////////////////////////////////////
    MAZE_CORE_API GeoLocation StringToGeoLocation(CString _str, Size _count)
    {
        for (std::map<GeoLocation, GeoLocationData>::const_iterator  it = c_geoLocations.begin(),
            end = c_geoLocations.end();
            it != end;
            ++it)
        {
            if (strncmp(it->second.name, _str, _count) == 0)
                return it->first;
        }

        return GeoLocation::Unknown;
    }

    //////////////////////////////////////////
    MAZE_CORE_API S32 GeoLocationToCallingCode(GeoLocation _location)
    {
        std::map<GeoLocation, GeoLocationData>::const_iterator it = c_geoLocations.find(_location);
        if (it != c_geoLocations.end())
            return it->second.callingCode;

        return -1;
    }

    //////////////////////////////////////////
    MAZE_CORE_API GeoLocation CallingCodeToGeoLocation(S32 _code)
    {
        for (std::map<GeoLocation, GeoLocationData>::const_iterator  it = c_geoLocations.begin(),
                                                                end = c_geoLocations.end();
                                                                it != end;
                                                                ++it)
        {
            if (it->second.callingCode == _code)
                return it->first;
        }

        return GeoLocation::Unknown;
    }

    //////////////////////////////////////////
    MAZE_CORE_API CString GeoLocationToISO2(GeoLocation _location)
    {
        std::map<GeoLocation, GeoLocationData>::const_iterator it = c_geoLocations.find(_location);
        if (it != c_geoLocations.end())
            return it->second.iso2;

        return "";
    }
    
    //////////////////////////////////////////
    MAZE_CORE_API GeoLocation ISO2ToGeoLocation(CString _str)
    {
        for (std::map<GeoLocation, GeoLocationData>::const_iterator  it = c_geoLocations.begin(),
                                                                end = c_geoLocations.end();
                                                                it != end;
                                                                ++it)
        {
            if (strcmp(it->second.iso2, _str) == 0)
                return it->first;
        }

        return GeoLocation::Unknown;
    }

    //////////////////////////////////////////
    MAZE_CORE_API CString GeoLocationToISO3(GeoLocation _location)
    {
        std::map<GeoLocation, GeoLocationData>::const_iterator it = c_geoLocations.find(_location);
        if (it != c_geoLocations.end())
            return it->second.iso3;

        return "";
    }
    
    //////////////////////////////////////////
    MAZE_CORE_API GeoLocation ISO3ToGeoLocation(CString _str)
    {
        for (std::map<GeoLocation, GeoLocationData>::const_iterator  it = c_geoLocations.begin(),
                                                                end = c_geoLocations.end();
                                                                it != end;
                                                                ++it)
        {
            if (strcmp(it->second.iso3, _str))
                return it->first;
        }

        return GeoLocation::Unknown;
    }


    //////////////////////////////////////////
    // Class SerializableGeoLocation
    //
    //////////////////////////////////////////
    String SerializableGeoLocation::toString() const
    {
        return GeoLocationToString(m_value);
    }

    //////////////////////////////////////////
    void SerializableGeoLocation::setString(CString _data, Size _count)
    {
        if (!_data)
        {
            m_value = GeoLocation::Unknown;
            return;
        }

        m_value = StringToGeoLocation(_data, _count);
    }

} // namespace Maze
//////////////////////////////////////////
