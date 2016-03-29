#include "lsrp-jacker.hpp"

static int comp(const void *a, const void *b)
{
	const char *pa = (const char*)a;
	const char *pb = (const char*)b;

	return
		(*pa > *pb) ? 1 :
		(*pa < *pb) ? -1 :
		0;
}

static void unscramble(const char *text)
{
	char buf[128];
	char text_arr[16];
	int unscrambled_word = -1;

	// The list of all possible hotwire strings and their sorted versions.
	const char *strings[][2] = {
		{ "FLOAT","AFLOT" },
		{ "COOKIE","CEIKOO" },
		{ "HEART","AEHRT" },
		{ "TEST","ESTT" },
		{ "EVADE","ADEEV" },
		{ "DEVICE","CDEEIV" },
		{ "VACUUM","ACMUUV" },
		{ "GOON","GNOO" },
		{ "ACCENT","ACCENT" },
		{ "AWAY","AAWY" },
		{ "WRIST","IRSTW" },
		{ "ORAL","ALOR" },
		{ "HACK","ACHK" },
		{ "IGNORE","EGINOR" },
		{ "CHARM","ACHMR" },
		{ "HIRE","EHIR" },
		{ "MARCH","ACHMR" },
		{ "BEAT","ABET" },
		{ "HEAT","AEHT" },
		{ "ROOT","OORT" },
		{ "WIRE","EIRW" },
		{ "PEAK","AEKP" },
		{ "FLOWER","EFLORW" },
		{ "HAZEL","AEHLZ" },
		{ "MEAT","AEMT" },
		{ "REFILL","EFILLR" },
		{ "EXACT","ACETX" },
		{ "LYRIC","CILRY" },
		{ "LOAF","AFLO" },
		{ "FROST","FORST" },
		{ "CROW","CORW" },
		{ "AMUSE","AEMSU" },
		{ "RACIST","ACIRST" },
		{ "OXYGEN","EGNOXY" },
		{ "DOOR","DOOR" },
		{ "SEXY","ESXY" },
		{ "HITCH","CHHIT" },
		{ "GRAND","ADGNR" },
		{ "RIPE","EIPR" },
		{ "MYTH","HMTY" },
		{ "PAGER","AEGPR" },
		{ "COOK","CKOO" },
		{ "BUTTON","BNOTTU" },
		{ "DODGE","DDEGO" },
		{ "FLUTE","EFLTU" },
		{ "HUMOR","HMORU" },
		{ "BOOZE","BEOOZ" },
		{ "TROJAN","AJNORT" },
		{ "THEORY","EHORTY" },
		{ "FLIP","FILP" },
		{ "PENIS","EINPS" },
		{ "BEAR","ABER" },
		{ "BONE","BENO" },
		{ "BUDGE","BDEGU" },
		{ "BURST","BRSTU" },
		{ "LADY","ADLY" },
		{ "ORANGE","AEGNOR" },
		{ "PAIN","AINP" },
		{ "BULLET","BELLTU" },
		{ "WOOL","LOOW" },
		{ "ORANGE","AEGNOR" },
		{ "HELMET","EEHLMT" },
		{ "FORK","FKOR" },
		{ "PRIDE","DEIPR" },
		{ "ROOT","OORT" },
		{ "SALE","AELS" },
		{ "BAND","ABDN" },
		{ "SENIOR","EINORS" },
		{ "ANNUAL","AALNNU" },
		{ "ALBUM","ABLMU" },
		{ "ALONE","AELNO" },
		{ "HYDRA","ADHRY" },
		{ "CURVE","CERUV" },
		{ "DATE","ADET" },
		{ "GHOST","GHOST" },
		{ "BUCKLE","BCEKLU" },
		{ "SEASON","AENOSS" },
		{ "DODGE","DDEGO" },
		{ "DOLL","DLLO" },
		{ "BOAT","ABOT" },
		{ "HORNY","HNORY" },
		{ "HOLE","EHLO" },
		{ "WINE","EINW" },
		{ "HUMAN","AHMNU" },
		{ "WIND","DINW" },
		{ "FOUR","FORU" },
		{ "RIDE","DEIR" },
		{ "FANCY","ACFNY" },
		{ "SPOON","NOOPS" },
		{ "GIRL","GILR" },
		{ "FLOWER","EFLORW" },
		{ "FACE","ACEF" },
		{ "HURT","HRTU" },
		{ "ELBOW","BELOW" },
		{ "HEIR","EHIR" },
		{ "LUMBER","BELMRU" },
		{ "FLAT","AFLT" },
		{ "PART","APRT" },
		{ "ORANGE","AEGNOR" },
		{ "KNIFE","EFIKN" },
		{ "DINNER","DEINNR" },
		{ "GRAPE","AEGPR" },
		{ "VERBAL","ABELRV" },
		{ "PAIN","AINP" },
		{ "MONEY","EMNOY" },
		{ "FLIP","FILP" },
		{ "SAMPLE","AELMPS" },
		{ "COOK","CKOO" },
		{ "GODLY","DGLOY" },
		{ "SPINE","EINPS" },
		{ "MALE","AELM" },
		{ "HEEL","EEHL" },
		{ "ENTIRE","EEINRT" },
		{ "REFUND","DEFNRU" },
		{ "ALARM","AALMR" },
		{ "TRAVEL","AELRTV" },
		{ "EDITOR","DEIORT" },
		{ "RATE","AERT" },
		{ "MOUSE","EMOSU" },
		{ "ANIMAL","AAILMN" },
		{ "HOOKAH","AHHKOO" },
		{ "SUNDAY","ADNSUY" },
		{ "PATH","AHPT" },
		{ "GROUND","DGNORU" },
		{ "CAFE","ACEF" },
		{ "HOME","EHMO" },
		{ "LINE","EILN" },
		{ "BEEF","BEEF" },
		{ "LEAGUE","AEEGLU" },
		{ "CHIMP","CHIMP" },
		{ "DRUNK","DKNRU" },
		{ "LOOP","LOOP" },
		{ "TURKEY","EKRTUY" },
		{ "DRIVE","DEIRV" },
		{ "GRAIN","AGINR" },
		{ "DIRT","DIRT" },
		{ "ACCEPT","ACCEPT" },
		{ "ZONE","ENOZ" },
		{ "BUSH","BHSU" },
		{ "CIRCUS","CCIRSU" },
		{ "SPIDER","DEIPRS" },
		{ "BARE","ABER" },
		{ "TRAP","APRT" },
		{ "RENT","ENRT" },
		{ "SAINT","AINST" },
		{ "YELL","ELLY" },
		{ "CRAWL","ACLRW" },
		{ "YOGA","AGOY" },
		{ "SALE","AELS" },
		{ "HATER","AEHRT" },
		{ "FLOOR","FLOOR" },
		{ "ROOF","FOOR" },
		{ "BRAKE","ABEKR" },
		{ "CRATE","ACERT" },
		{ "DATE","ADET" },
		{ "TAME","AEMT" },
		{ "HYDRA","ADHRY" },
		{ "COKE","CEKO" },
		{ "OBESE","BEEOS" },
		{ "DROP","DOPR" },
		{ "FLIRT","FILRT" },
		{ "DRUNK","DKNRU" },
		{ "SWEAT","AESTW" },
		{ "DRUNK","DKNRU" },
		{ "ENJOY","EJNOY" },
		{ "DODGY","DDGOY" },
		{ "DINER","DEINR" },
		{ "COURT","CORTU" },
		{ "BUMBLE","BBELMU" },
		{ "CHEEK", "CEEHK" },
		{ "SOAP", "AOPS" },
		{ "PACE", "APEC" },
		{ "DRIP", "DIPR" },
		{ "RAIN", "AINR" },
		{ "CORE", "CEOR" },
		{ "SOUR", "ORSU" },
		{ "HOUR", "HORU" },
		{ "GRACE", "ACEGR" },
		{ "TRIM", "IMRT" },
		{ "CHINA", "ACHIN" },
		{ "DAMNED", "ADDEMN" },
		{ "SUFFER", "EFFRSU" },
		{ "BEARD", "ABDER" },
		{ "DRIP", "DIPR" },
		{ "EASTER", "AEERST" },
		{ "PASS", "APSS" },
		{ "FLYER", "EFLRY" },
		{ "SCREW", "CERSW" },
		{ "TENSE", "EENTS" },
		{ "CHEF", "CEFH" },
		{ "BABE", "ABBE" },
		{ "TWIN", "INTW" },
		{ "BAKER", "ABEKR" },
		{ "EPIC", "CEIP" },
		{ "FIVE", "EFIV" },
		{ "CINEMA", "ACEIMN"},
		{ "FAVOUR", "AFORUV" },
		{ "HORSE", "EHORS" },
		{ "PERIOD", "DEIOPR"},
		{ "BEAM", "ABEM" },
		{ "VULGAR", "AGLRUV" },
		{ "LAMP", "ALMP" },
		{ "GIANT", "AGINT" },
		{ "SING", "GINS" },
		{ "AMISH", "AHIMS" },
		{ "PACE", "ACEP" },
		{ "METEOR", "EEMORT" },
		{ "AROUSE", "AEORSU" },
		{ "CAKE", "ACEK" },
		{ "TENSE", "EENTS" },
		{ "REFUSE", "EEFRSU" },
		{ "COLD", "CDLO" }
	};

	if (text[0] == '\0') return;

	// Copy the parameter (string literal) into an array for modification.
	strcpy_s(text_arr, 16, text);

	// Convert the input into upper-case.
	_strupr(text_arr);

	// Sort the characters alphabetically.
	qsort((void*)text_arr, strlen(text_arr), sizeof(char), comp);

	// Compare the computed string to the list of possible values.
	for (int i = 0; i < sizeof(strings) / sizeof(*strings); i++) {
		if (strlen(text_arr) != strlen(strings[i][1])) continue;

		if (!strcmp(text_arr, strings[i][1])) {
			unscrambled_word = i;
			break;
		}
	}

	if (unscrambled_word == -1)
		sprintf_s(buf, 128, "/uns %s", text_arr);
	else
		sprintf_s(buf, 128, "/uns %s", strings[unscrambled_word][0]);
	
	SF->getSAMP()->getPlayers()->pLocalPlayer->Say(buf);
	return;
}

bool CALLBACK hook_get_tdstring(stRakNetHookParams* params)
{
	short td_id;
	unsigned short td_len = 0;
	char td_text[1024];

	int sscanf_result;
	char unscramble_string[16];

	static char last_unscramble_string[16];

	if (params->packetId == RPC_ScrTextDrawSetString) {
		params->bitStream->ResetReadPointer();
		params->bitStream->Read(td_id);
		params->bitStream->Read(td_len);
		params->bitStream->Read(td_text, td_len);
		td_text[td_len] = '\0';

		if (!strncmp("~y~/(u", td_text, 6)) {
			sscanf_result = sscanf_s(td_text,
				"~y~/(uns)cramble ~w~<unscrambled word>~r~ to unscramble the word.~n~\"~w~%[^~]~r~\".",
				unscramble_string, 16);

			if (sscanf_result == 1 && strcmp(unscramble_string, last_unscramble_string) != 0) {
				strcpy_s(last_unscramble_string, 16, unscramble_string);
				unscramble(unscramble_string);
			}
		}
		params->bitStream->ResetReadPointer();
	}
	return true;
}