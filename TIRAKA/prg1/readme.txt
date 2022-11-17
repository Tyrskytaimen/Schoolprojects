Harjoitustyössä päätietorakenne, johon kaupungit tallennetaan on unordered map. Koska joudutaan suorittamaan paljon hakuja, vaikuttaa tämä järkevältä tietorakenteelta, sillä avaimen perusteella kaupungin etsiminen on keskimäärin vakioaikainen.
	Operaatioissa, joissa on hyödynnetty omaa funktiota towns_contains, on oletettu, että funktion tehokkuus on keskimäärin O(1). Koska esimerkiksi get_town_name, get_town_coordinates ja vastaavat funktiot eivät käytä muita kuin towns_contains -funktiota ja find -algoritmiä, on niiden tehokkuus arvioitu vakioaikaisiksi.
	Operaatiossa taxer_path ja longest_vassal_path käytetään algoritmiä reverse, jonka tehokkuus on N/2. Pahimmassa tapauksessa kuitenkin käydään läpi kaikki kaupungit, jolloin tehokkuus on pahimmassa tapauksessa lineaarinen.


	remove_town funktiosta pieni huomautus: se onnistuu poistamaan kaupungin, jolla on isäntäkaupunki, mutta selvittämättömästä syystä ohjelma kaatuu, jos yrittää poistaa isännättömän kaupungin.
