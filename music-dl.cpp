//Author - Aman-Rohilla@rohilla.co.in

#include <utils.h>
#include <help.h>

using namespace std;

int other=0;
HANDLE hrunning;
void mdl_exit()
{
	SetThreadExecutionState(ES_CONTINUOUS);
	console_cursor(1);

	del_file("processing\\found_tags.txt");
	del_file("processing\\new_songs.txt");
	del_file("processing\\new_albums.txt");
	del_file("processing\\new_playlists.txt");
	del_file("processing\\new_lyrics.txt");
	del_file("processing\\download_paths.txt");
	del_file("processing\\dl_covers_session.txt");
	del_file("processing\\temp_link.txt");
	del_file("processing\\progress.txt");
	if(!file_size("processing\\tags.txt"))
		del_file("processing\\tags.txt");

	CloseHandle(hrunning);

	if(!dir_content("processing",NULL,0,0,1)) remove_dir("processing");
}

int main(int argc, char* argv[])
{
	argcg = argc;
	argvg = argv;

	string sd = ext_dn(exe_path());
    string curr_dir = directory();
    curr_dir += "\\";

	if(!exists("C:\\Windows\\System32\\wbem\\WMIC.exe") || !exists(sd) || !writable_dir(sd) || !exists(sd+"sources\\tools\\aria2c.exe")
		|| !exists(sd+"sources\\tools\\kid3\\kid3-cli.exe")) return 0; 

	SetCurrentDirectoryA(sd.c_str());

 	if(arg_find("-h",1) || arg_find("--help",1))
 	{
    	cout<<R"(
    
	    Music Downloader By Aman-Rohilla@rohilla.co.in
______________________________________________________________

)";
 		cout<<help_str;

 		return 0;
 	}

	progress_bar_param pb("");
    string file, s, t, temp, link, query, dl_file;
     //source_dir
    int config=0;

    //else sd="\\";
    int start_x=0, start_y=0;

	if(exists("sources\\perma_config"))	config = 1;

	if(arg_find("--permanent-config"))
		CreateFileA("sources\\perma_config", GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, NULL);
	else if(arg_find("++permanent-config"))
		del_file("sources\\perma_config");

	if(arg_find("--config")) 		config = 1;
	else if(arg_find("++config")) config = 0;

	int xhgn=1;

	if(config)
	{
		if(find_in_text("sources\\config.txt","q=1",0,1)!="")	xhgn=0;
		if(find_in_text("sources\\config.txt","st=1",0,1)!="")	xhgn=1;

	}

	if(arg_find("-q"))	xhgn = 0;
	if(arg_find("--st"))	xhgn = 1;
	else if(arg_find("++st"))	xhgn = 0;

	int already_run=0;

    hrunning = CreateFileA("sources\\running", GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_FLAG_DELETE_ON_CLOSE, NULL);
    if(hrunning==INVALID_HANDLE_VALUE && GetLastError()==32) already_run=1;
    else if(hrunning==INVALID_HANDLE_VALUE) return 0;

	if(xhgn)
	{
		cout<<R"(
    
	    Music Downloader By Aman-Rohilla@rohilla.co.in
______________________________________________________________

)";
		cout<<endl;
 	}

 	SetConsoleTitleA("Music Downloader");
	if(already_run) exit_now(argc,"\tAnother instance of Music Downloader is ALREADY RUNNING...");
 	current_console_cursor(start_x,start_y);
	SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED);

    int vec_size=0;
    
    string dd; //download_dir;
    string rd; // rd is rename directory
    string serialize_dir;

    string json_source = "********";
    string extensions = "mp3 m4a aac flac mp4";
    string rename_script, tag_script;
    string play_list_name;
    string prk1, prk2, prk3, prk4;
    string pbcs="█", pbrs="░";
    string pb_transition=" -> ";

	pause_param pp(&pb,"");
	pp.title="Music Downloader";
	aria2_progress_bar_param apbps(&pb,"");
	exit__ exit(argc,"","",&pb,&pp,&apbps);	

    DWORD pid_ssapi;
    string rem_dup;
    int newrelaunch=0, re_down_pl=0, link_found=0, dl_status=1, proxy=1, basic_tags_done=0;
    int remove_duplicates = 1, strong=1, playlists=1, found=0;
    int retries=0, cover_retries=10, basic_tag_retries=10, song_retries=5;
    int json_song_retries = 5, json_album_retries=5, json_playlist_retries=5, json_lyrics_retries=5, json_timeout=600;
    int json_song_concurrency = 20, json_album_concurrecncy = 5, json_playlist_concurrecncy=2, json_lyrics_concurrency=10;
    int cover_concurrency = 20, basic_tag_concurrency=40, song_concurrency = 10;
    int i=0, lines=0, last=0;
    int tq=0, qjs=0, qja=0, qjp=0, qjl=0, qd=0, qc=0, qb=0;
    int pb_=0, pbjs=1, pbja=1, pbjp=1, pbjl=1, pbd=1, pbc=1, pbb=1;
    int qtagging=0, qsed=0, qrenaming=0, qdir_serializing=0, qdynamic=1;
    int basic_processing=1, tag_processing=1, download_processing=1, rename_processing=0, serialize_dir_processing=0, specified_processing=0;
   	int remove_words=1, flag_console_cursor=0, progress_delay=1000, percentage_gap=1;
   	double serialize_dir_size=10;
   	int st=1, basic_info=1, serialize_dir_nof=1000, replace_=1, flag_pause_resume=1;
   	int mb=0, dmb=1, pb_thread=1; //pause_message, delayed_pause_message
   	int other_files=0, renaming=1;
   	int pldf=0, renaming_failed=0, dir_ren_failed=0;
	int scan_earlier=1, scan_earlier_i=1;

	string only_one_song;

    string params_json_songs 	 = " --download-result=hide --summary-interval=0";
    string params_json_albums 	 = " --download-result=hide --summary-interval=0";
    string params_json_playlists = " --download-result=hide --summary-interval=0";
    string params_json_lyrics	 = " --download-result=hide --summary-interval=0";
    string params_basic_tags	 = " --download-result=hide --summary-interval=0";
    string params_covers		 = " --download-result=hide --summary-interval=0";
    string params_song 			 = " --download-result=hide --summary-interval=0";

    string ddi;
    int newrelaunch_i=0, rename_processing_i=0, re_down_pl_i=0;
    if(argc==1)
    {
  		string temp;
  		cout<<"\tEnter query as song/album/playlist link or file name containing links/webpage_body or --last to continue earliar downloads or --help for flags and more options.";
    	cout<<"\n\n\tQuery : ";
        getline(cin,query); if (cin.fail()) { CloseHandle(hrunning); return 0; }
        if(query=="--help")
        {
            CloseHandle(hrunning);

            console_pointer("etls "+to_string(start_x));
            cout<<help_str<<endl;
            cout<<"\n\tPress Enter to EXIT...";
            getline(cin,s);
            return 0;
        }

    	cout<<"\n\tEnter 'y' for setting redownload flag as 1                 : ";
    	getline(cin,s);     if (cin.fail()) { CloseHandle(hrunning); return 0; }
    	if(s=="y" || s=="Y")	newrelaunch_i = 1;

    	cout<<"\tEnter 'y' for setting redownload-playlists flag as 1       : ";
    	getline(cin,s);     if (cin.fail()) { CloseHandle(hrunning); return 0; }
    	if(s=="y" || s=="Y")	re_down_pl_i = 1;

		cout<<"\tEnter 'y' for not scanning earlier downloads               : ";
    	getline(cin,s);     if (cin.fail()) { CloseHandle(hrunning); return 0; }
    	if(s=="y" || s=="Y")	scan_earlier_i = 0;

    	cout<<"\tEnter download directory path (default="+sd+"downloaded\\) : ";
    	getline(cin,s);    if (cin.fail()) { CloseHandle(hrunning); return 0; }
    	if(str_find_count(s," ")!=s.length())	ddi = s;

    	console_pointer("etls "+to_string(start_x));
    }

    if(config)
    {
    	int lines=0;
    	string * config_arr = text_to_string_array("sources\\config.txt",&lines);
    	int i=0;

    	if(lines)
    	{
    		for(int i=0; i<lines; i++)
    		config_arr[i] = replace_string_all(config_arr[i]," ","");

    		psv_sorter(config_arr, lines, 1);

    		///////// string valued params started

			i = binary_search_string(config_arr,0,lines,"dd",1);
			if(i!=-1)	dd = get_column(config_arr[i],2,'=');

            i = binary_search_string(config_arr,0,lines,"remove-duplicates",1);
            if(i!=-1)   rem_dup = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"dir-ser-dir-name",1);
			if(i!=-1)	serialize_dir = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"json-source",1);
			if(i!=-1)	json_source = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"extensions",1);
			if(i!=-1)	extensions = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"dir-ren-dir-name",1);
			if(i!=-1)	rd = config_arr[i];

			i = binary_search_string(config_arr,0,lines,"dir-ren-format",1);
			if(i!=-1)	rename_script = config_arr[i];

			rename_script = erase_find(rename_script,"=");
			
			i = binary_search_string(config_arr,0,lines,"renaming-format",1);
			if(i!=-1)	tag_script = config_arr[i];

			tag_script = erase_find(tag_script,"=");

			i = binary_search_string(config_arr,0,lines,"playlist-name",1);
			if(i!=-1)	play_list_name = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"prk1",1);
			if(i!=-1)	prk1 = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"prk2",1);
			if(i!=-1)	prk2 = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"prk3",1);
			if(i!=-1)	prk3 = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"prk4",1);
			if(i!=-1)	prk4 = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"pbcs",1);
			if(i!=-1)	pbcs = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"pbrs",1);
			if(i!=-1)	pbrs = get_column(config_arr[i],2,'=');

			i = binary_search_string(config_arr,0,lines,"pb-transition",1);
			if(i!=-1)	pb_transition = get_column(config_arr[i],2,'=');

			///////// string valued params ended

    		//////// true/false params started

			i = binary_search_string(config_arr,0,lines,"q",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") tq = 1; else tq = 0;}

			if(tq)
		    {
				qjs=1; qja=1; qjp=1; qjl=1; qd=1; qc=1; qb=1;
		    	pbjs=0; pbja=0; pbjp=0; pbjl=0; pbd=0; pbc=0; pbb=0;
		    	qtagging=1; qsed=1; qrenaming=1; qdir_serializing=1, st=0; basic_info=0;
		    }

			i = binary_search_string(config_arr,0,lines,"done-msg",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") basic_info = 1; else basic_info = 0;}

			i = binary_search_string(config_arr,0,lines,"pb",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pb_ = 1; else pb_ = 0;}

			if(pb_)
		    {
				qjs=0; qja=0; qjp=0; qjl=0; qd=0; qc=0; qb=0;
		    	pbjs=1; pbja=1; pbjp=1; pbja=1; pbd=1; pbc=1; pbb=1;
		    }

			i = binary_search_string(config_arr,0,lines,"percentage-gap",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") percentage_gap = 1; else percentage_gap = 0;}

			i = binary_search_string(config_arr,0,lines,"redownload",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") newrelaunch = 1; else newrelaunch = 0;}

			i = binary_search_string(config_arr,0,lines,"redownload-playlist",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") re_down_pl = 1; else re_down_pl = 0;}

			i = binary_search_string(config_arr,0,lines,"proxy",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") proxy = 1; else proxy = 0;}

			i = binary_search_string(config_arr,0,lines,"basic-tags-done",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") basic_tags_done = 1; else basic_tags_done = 0;}

			i = binary_search_string(config_arr,0,lines,"playlists",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") playlists = 1; else playlists = 0;}

			i = binary_search_string(config_arr,0,lines,"last",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") last = 1; else last = 0;}

			i = binary_search_string(config_arr,0,lines,"qjs",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qjs = 1; else qjs = 0;}

			i = binary_search_string(config_arr,0,lines,"qja",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qja = 1; else qja = 0;}

			i = binary_search_string(config_arr,0,lines,"qjp",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qjp = 1; else qjp = 0;}

			i = binary_search_string(config_arr,0,lines,"qjl",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qjl = 1; else qjl = 0;}

			i = binary_search_string(config_arr,0,lines,"qd",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qd = 1; else qd = 0;}

			i = binary_search_string(config_arr,0,lines,"qc",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qc = 1; else qc = 0;}

			i = binary_search_string(config_arr,0,lines,"qb",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qb = 1; else qb = 0;}

			i = binary_search_string(config_arr,0,lines,"pbjs",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pbjs = 1; else pbjs = 0;	if(pbjs)	qjs=0;}

			i = binary_search_string(config_arr,0,lines,"pbja",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pbja = 1; else pbja = 0;	if(pbja)	qja=0;}

			i = binary_search_string(config_arr,0,lines,"pbjp",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pbjp = 1; else pbjp = 0;	if(pbjp)	qjp=0;}

			i = binary_search_string(config_arr,0,lines,"pbjl",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pbjl = 1; else pbjl = 0;	if(pbjl)	qjl=0;}

			i = binary_search_string(config_arr,0,lines,"pbd",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pbd = 1; else pbd = 0;	if(pbd)		qd=0;}

			i = binary_search_string(config_arr,0,lines,"pbc",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pbc = 1; else pbc = 0;	if(pbc)		qc=0;}

			i = binary_search_string(config_arr,0,lines,"pbb",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pbb = 1; else pbb = 0;	if(pbb)		qb=0;}

			i = binary_search_string(config_arr,0,lines,"qtagging",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qtagging = 1; else qtagging = 0;}

            i = binary_search_string(config_arr,0,lines,"qscanning-earlier-downloads",1);
            if(i!=-1)   { if(get_column(config_arr[i],2,'=')=="1") qsed = 1; else qsed = 0;}

			i = binary_search_string(config_arr,0,lines,"scanning-earlier-downloads",1);
            if(i!=-1)   { if(get_column(config_arr[i],2,'=')=="1") scan_earlier = 1; else scan_earlier = 0;}

			i = binary_search_string(config_arr,0,lines,"qdir-ren",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qrenaming = 1; else qrenaming = 0;}

			i = binary_search_string(config_arr,0,lines,"qdir-ser",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qdir_serializing = 1; else qdir_serializing = 0;}

			i = binary_search_string(config_arr,0,lines,"c",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") qdynamic = 1; else qdynamic = 0;}


			i = binary_search_string(config_arr,0,lines,"renaming",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") renaming = 1; else renaming = 0;}


			i = binary_search_string(config_arr,0,lines,"remove-words",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") remove_words = 1; else remove_words = 0;}

			i = binary_search_string(config_arr,0,lines,"console-cursor",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") flag_console_cursor = 1; else flag_console_cursor = 0;}

			i = binary_search_string(config_arr,0,lines,"correct-playlist-name",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") replace_ = 1; else replace_ = 0;}

			i = binary_search_string(config_arr,0,lines,"pause-resume",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") flag_pause_resume = 1; else flag_pause_resume = 0;}

			i = binary_search_string(config_arr,0,lines,"pause-message",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") mb = 1; else mb = 0;}

			i = binary_search_string(config_arr,0,lines,"delayed-pause-message",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") dmb = 1; else dmb = 0;}

			i = binary_search_string(config_arr,0,lines,"pb-thread",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") pb_thread = 1; else pb_thread = 0;}

			i = binary_search_string(config_arr,0,lines,"other-files",1);
			if(i!=-1)	{ if(get_column(config_arr[i],2,'=')=="1") other_files = 1; else other_files = 0;}

			//////// true/false params ended


			//////// integer valued params started

			i = binary_search_string(config_arr,0,lines,"cover-retries",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) cover_retries = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"basic-tag-retries",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) basic_tag_retries = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"song-retries",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) song_retries = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-song-retries",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_song_retries = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-lyrics-retries",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_lyrics_retries = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-album-retries",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_album_retries = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-playlist-retries",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_playlist_retries = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-timeout",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_timeout = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-song-concurrency",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_song_concurrency = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-album-concurrecncy",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_album_concurrecncy = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-playlist-concurrecncy",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_playlist_concurrecncy = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"json-lyrics-concurrency",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) json_lyrics_concurrency = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"cover-concurrency",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) cover_concurrency = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"basic-tag-concurrency",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) basic_tag_concurrency = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"song-concurrency",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) song_concurrency = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"progress-delay",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) progress_delay = stoi(get_column(config_arr[i],2,'=')); }

			i = binary_search_string(config_arr,0,lines,"dir-ser-dir-size",1);
			if(i!=-1)	serialize_dir_size = stod(get_column(config_arr[i],2,'='));

			i = binary_search_string(config_arr,0,lines,"dir-ser-nof",1);
			if(i!=-1)	{ if(is_int(get_column(config_arr[i],2,'='))) serialize_dir_nof = stoi(get_column(config_arr[i],2,'=')); }

			//////// integer valued params ended
    	}
    	
    }




    if(argc!=1)
    {
    	query = argv[1];
    	
	    if(arg_find("-q",1))
	    {
			qjs=1; qja=1; qjp=1; qd=1; qc=1; qb=1, qjl=1;
	    	pbjs=0; pbja=0; pbjp=0; pbd=0; pbc=0; pbb=0, pbjl=0;
	    	qtagging=1; qsed=1; qrenaming=1; qdir_serializing=1; basic_info=0;
	    }
	    else if(arg_find("+q",1))
	    {
			qjs=0; qja=0; qjp=0; qd=0; qc=0; qb=0, qjl=0;
	    	pbjs=1; pbja=1; pbjp=1; pbd=1; pbc=1; pbb=1, pbjl=1;
	    	qtagging=0; qsed=0; qrenaming=0; qdir_serializing=0; basic_info=1;
	    }


	    if(arg_find("--done-msg",1))       basic_info = 1;
	    else if(arg_find("++done-msg",1))  basic_info = 0;

		if(arg_find("--pb",1))
	    {
			qjs=0; qja=0; qjp=0; qjl=0; qd=0; qc=0; qb=0;
	    	pbjs=1; pbja=1; pbjp=1; pbja=1; pbd=1; pbc=1; pbb=1;
	    }
		else if(arg_find("++pb",1))
	    {
			qjs=0; qja=0; qjp=0; qjl=0; qd=0; qc=0; qb=0;
	    	pbjs=0; pbja=0; pbjp=0; pbja=0; pbd=0; pbc=0; pbb=0;
	    }


    	arg_value("--dd",&dd);
    	arg_value("--dir-ren-dir-name",&rd);
    	arg_value("--dir-ser-dir-name",&serialize_dir);
    	arg_value("--json-source",&json_source);
    	arg_value("--extensions",&extensions);
    	arg_value("--dir-ren-format",&rename_script);
    	arg_value("--renaming-format",&tag_script);
		arg_value("--prk1",&prk1);
		arg_value("--prk2",&prk2);
		arg_value("--prk3",&prk3);
		arg_value("--prk4",&prk4);
	    arg_value("--pbcs",&pbcs);
	    arg_value("--pbrs",&pbrs);
    	arg_value("--playlist-name",&play_list_name);
        arg_value("--pb-transition",&pb_transition);
	    arg_value("--remove-duplicates",&rem_dup);



    	if(arg_find("--percentage-gap",1))		percentage_gap = 1;
   else if(arg_find("++percentage-gap",0))		percentage_gap = 0;

    	if(arg_find("--redownload",1))			newrelaunch = 1;
   else if(arg_find("++redownload",1))			newrelaunch = 0;

    	if(arg_find("--redownload-playlist",1))	re_down_pl = 1;
   else if(arg_find("++redownload-playlist",1))	re_down_pl = 0;

    	if(arg_find("--proxy",1))				proxy = 1;
   else if(arg_find("++proxy",1))				proxy = 0;

    	if(arg_find("--basic-tags-done",1))		basic_tags_done = 1;
   else if(arg_find("++basic-tags-done",1))		basic_tags_done = 0;

    	if(arg_find("--playlists",1))			playlists = 1;
   else if(arg_find("++playlists",1))			playlists = 0;

    	if(arg_find("--last",1))				last = 1;

    	if(arg_find("--qjs",1))					qjs = 1;
   else if(arg_find("++qjs",1))					qjs = 0;

    	if(arg_find("--qja",1))					qja = 1;
   else if(arg_find("++qja",1))					qja = 0;

    	if(arg_find("--qjp",1))					qjp = 1;
   else if(arg_find("++qjp",1))					qjp = 0;

    	if(arg_find("--qjl",1))					qjl = 1;
   else if(arg_find("++qjl",1))					qjl = 0;

    	if(arg_find("--qd",1))					qd = 1;
   else if(arg_find("++qd",1))					qd = 0;

    	if(arg_find("--qc",1))					qc = 1;
   else if(arg_find("++qc",1))					qc = 0;

    	if(arg_find("--qb",1))					qb = 1;
   else if(arg_find("++qb",1))					qb = 0;

    	if(arg_find("--pbjs",1))				{ pbjs = 1; qjs = 0; }
   else if(arg_find("++pbjs",1))				  pbjs = 0;

    	if(arg_find("--pbja",1))				{ pbja = 1; qja = 0; }
   else if(arg_find("++pbja",1))				  pbja = 0;

    	if(arg_find("--pbjp",1))				{ pbjp = 1; qjp = 0; }
   else if(arg_find("++pbjp",1))				  pbjp = 0;

    	if(arg_find("--pbjl",1))				{ pbjl = 1; qjl = 0; }
   else if(arg_find("++pbjl",1))				  pbjl = 0;

    	if(arg_find("--pbd",1))					{ pbd = 1; qd = 0; }
   else if(arg_find("++pbd",1))					  pbd = 0;

    	if(arg_find("--pbc",1))					{ pbc = 1; qc = 0; }
   else if(arg_find("++pbc",1))					  pbc = 0;

    	if(arg_find("--pbb",1))					{ pbb = 1; qb = 0; }
   else if(arg_find("++pbb",1))					  pbb = 0;

    	if(arg_find("--qtagging",1))			qtagging = 1;
   else if(arg_find("++qtagging",1))			qtagging = 0;

        if(arg_find("--qscanning-earlier-downloads",1)) qsed = 1;
   else if(arg_find("++qscanning-earlier-downloads",1)) qsed = 0;

        if(arg_find("--scanning-earlier-downloads",1)) scan_earlier = 1;
   else if(arg_find("++scanning-earlier-downloads",1)) scan_earlier = 0;

    	if(arg_find("--qdir-ren",1))				qrenaming = 1;
   else if(arg_find("++qdir-ren",1))				qrenaming = 0;

    	if(arg_find("--qdir-ser",1))				qdir_serializing = 1;
   else if(arg_find("++qdir-ser",1))				qdir_serializing = 0;

    	if(arg_find("-c",1))						qdynamic = 1;
   else if(arg_find("+c",1))						qdynamic = 0;

    	if(arg_find("--renaming",1))				renaming = 1;
   else if(arg_find("++renaming",1))				renaming = 0;

    	if(arg_find("--dir-ren",1))					rename_processing = 1;

    	if(arg_find("--dir-ser",1)) 				serialize_dir_processing = 1;

    	if(arg_find("--remove-words",1))			remove_words = 1;
   else if(arg_find("++remove-words",1))			remove_words = 0;

    	if(arg_find("--console-cursor",1))			flag_console_cursor = 1;
   else if(arg_find("++console-cursor",1))			flag_console_cursor = 0;

    	if(arg_find("--correct-playlist-name",1))	replace_ = 1;
   else if(arg_find("++correct-playlist-name",1))	replace_ = 0;

    	if(arg_find("--pause-resume",1))			flag_pause_resume = 1;
   else if(arg_find("++pause-resume",1))			flag_pause_resume = 0;

    	if(arg_find("--pb-thread",1))				pb_thread = 1;
   else if(arg_find("++pb-thread",1))				pb_thread = 0;

    	if(arg_find("--other-files",1))				other_files = 1;
   else if(arg_find("++other-files",1))				other_files = 0;


    	arg_as_int("--cover-retries",&cover_retries);
    	arg_as_int("--basic-tag-retries",&basic_tag_retries); 
    	arg_as_int("--song-retries",&song_retries);  
    	arg_as_int("--json-song-retries",&json_song_retries); 
    	arg_as_int("--json-lyrics-retries",&json_lyrics_retries); 
    	arg_as_int("--json-album-retries",&json_album_retries); 
    	arg_as_int("--json-playlist-retries",&json_playlist_retries); 
    	arg_as_int("--json-timeout",&json_timeout);  
    	arg_as_int("--json-song-concurrency",&json_song_concurrency); 
    	arg_as_int("--json-album-concurrecncy",&json_album_concurrecncy); 
    	arg_as_int("--json-playlist-concurrecncy",&json_playlist_concurrecncy);
    	arg_as_int("--json-lyrics-concurrency",&json_lyrics_concurrency);
    	arg_as_int("--cover-concurrency",&cover_concurrency);
    	arg_as_int("--basic-tag-concurrency",&basic_tag_concurrency);
    	arg_as_int("--song-concurrency",&song_concurrency);
    	arg_as_int("--progress-delay",&progress_delay);
    	arg_as_int("--dir-ser-nof",&serialize_dir_nof);

        arg_value("--dir-ser-dir-size",&s);
        if(s!="") serialize_dir_size = stod(s);
    }
    link = query;

    md_if("processing");
    md_if("sources\\json");
    md_if("sources\\covers");
    md_if("sources\\album_bodies");
    md_if("sources\\lyrics");
    md_if("sources\\logs");

    //console_cursor(flag_console_cursor);

    if(ddi!="")	dd = ddi;
    if(dd=="")	dd = sd+"downloaded\\";

    if(curr_dir!=sd) SetCurrentDirectoryA(curr_dir.c_str());
    
    if(!str_find(link,"/")) link = abs_path(link);
    dd = abs_path(dd);
    if(rd!="") rd = abs_path(rd);
    if(serialize_dir!="") serialize_dir = abs_path(serialize_dir);

    if(curr_dir!=sd) SetCurrentDirectoryA(sd.c_str());

    if(argc==1)
    {
    	newrelaunch = newrelaunch_i;
    	re_down_pl  = re_down_pl_i;
		scan_earlier = scan_earlier_i;
    }

    if(dd.length() && dd[dd.length()-1]=='\\')	dd.erase(dd.length()-1,1);

    string ddt = dd+"__temp";

    if(dd.length())  dd += "\\";

    string sda = erase_rfind_found_to_end(sd,"\\"); //source directory name for aria2c commands
    string dda = ddt; //download directory name for aria2c commands
    string pbfile = "processing\\progress.txt";
    ddt += "\\";

    if(qjp)	pbjp = 0;
    if(qjs)	pbjs = 0;
    if(qja)	pbja = 0;
    if(qjl)	pbjl = 0;
    if(qb)	pbb = 0;
    if(qc)	pbc = 0;
    if(qd)	pbd = 0;

    if(pbjp)	params_json_playlists = params_json_playlists +" > "+pbfile;
    if(pbjs)	params_json_songs	  = params_json_songs	  +" > "+pbfile;
    if(pbja)	params_json_albums	  = params_json_albums 	  +" > "+pbfile;
    if(pbjl)	params_json_lyrics 	  = params_json_lyrics    +" > "+pbfile;
    if(pbb)		params_basic_tags     = params_basic_tags 	  +" > "+pbfile;
    if(pbc)		params_covers 		  = params_covers 		  +" > "+pbfile;
    if(pbd)		params_song 		  = params_song           +" > "+pbfile;


    if(qjp)	params_json_playlists +=" -q";
    if(qjs)	params_json_songs 	  +=" -q";
    if(qja)	params_json_albums 	  +=" -q";
    if(qjl)	params_json_lyrics 	  +=" -q";
    if(qb)	params_basic_tags 	  +=" -q";
    if(qc)	params_covers 		  +=" -q";
    if(qd)	params_song 		  +=" -q";

    if(json_source=="********") json_source = "********";

    if(json_source!="********" && json_source!="********" && json_source!="********")	json_source = "********";
    if(extensions=="")	extensions = "mp3 m4a mp4 aac flac";
    
    if(tag_script=="")	 tag_script = "|album| - |title|";
 
    if(abs(cover_retries)>20)			cover_retries 			= 20;
    if(abs(basic_tag_retries)>20)	  	basic_tag_retries 		= 20;
    if(abs(song_retries)>20)			song_retries 			= 20;
    if(abs(json_lyrics_retries)>20)		json_lyrics_retries		= 20;
    if(abs(json_album_retries)>20)		json_album_retries		= 20;
    if(abs(json_playlist_retries)>20)	json_playlist_retries	= 20;
    if(abs(json_lyrics_retries)>20)		json_lyrics_retries		= 20;
    if(abs(json_timeout)>600)			json_timeout			= 20;
    if(abs(json_song_concurrency)>100)	json_song_concurrency	= 100;
    if(abs(json_album_concurrecncy)>50)	json_album_concurrecncy = 50;
    if(abs(json_playlist_concurrecncy)>50)json_playlist_concurrecncy=50;
    if(abs(json_lyrics_concurrency)>100)json_lyrics_concurrency = 100;
    if(abs(cover_concurrency)>100)		cover_concurrency		= 100;
    if(abs(basic_tag_concurrency)>200)	basic_tag_concurrency	= 200;
    if(abs(song_concurrency)>50)		song_concurrency 		= 50;

    //////////// setting variables

    if(rem_dup=="") rem_dup = "strong";

    if(rem_dup=="off") remove_duplicates=0;
    if(rem_dup=="normal") strong=0;

    play_list_name = validate_path(play_list_name);
	if(rename_processing || serialize_dir_processing)
	{
		basic_processing = 0;
		download_processing = 0;
		tag_processing = 0;
		if(rename_processing) serialize_dir_processing=0;
	}
    else if(!network_connection()) exit_now(argc,"\tNETWORK DISCONNECTED...");

    pb_transition = replace_string_all(pb_transition,"\\\\n","\n");
    pb_transition = replace_string_all(pb_transition,"\\\\t","\t");

    apbps.progress_file	 = sd+"processing\\progress.txt";
	pb.clear 		  	 = qdynamic;
	pb.cursor 		  	 = flag_console_cursor;
	pb.percentage_gap 	 = percentage_gap;
	pb.completed  		 = pbcs;
	pb.remaining  		 = pbrs;
	pb.transition  		 = pb_transition;
	pb.thread			 = 0;

	del_file("processing\\progress.txt");

	pause_param *ppp=NULL;
	if(flag_pause_resume)
	{
		pp.execute=1;
		if(dmb)	pp.delayed_message = 1;
		if(mb) 	pp.message = 1;

		vector<int> v;

		v.push_back(hex2i(prk1));
		v.push_back(hex2i(prk2));
		v.push_back(hex2i(prk3));
		v.push_back(hex2i(prk4));

		sort(v.begin(),v.end(),greater<>());

		pp.k1 = v[0];
		pp.k2 = v[1];
		pp.k3 = v[2];
		pp.k4 = v[3];

		if(!pp.k1)
		{
			pp.k1 = hex2i("0x11");
			pp.k2 = hex2i("0x10");
		}
		v.clear();

		ppp = &pp;
	}

    if(ext_fn(link)=="--last")
    {
    	link="processing\\tags.txt";
    	basic_processing = 0;
    	if(!exists(link))
    	{
    		mdl_exit();
    		exit.exit_now("\t"+sd+link+" DOESN'T EXIST.");
    	}
    }

	if(flag_pause_resume) pp.launch();

    vector<string> source_url;


if(basic_processing)
{
    if(find_in_text(link,"<!DOCTYPE HTML>")!="" || find_in_text(link,"<body>")!="")
    {
    	vector<string> links_vec;
    	ss_le(link,links_vec);
    	vector_to_text(out__file(link,"_links"),links_vec,0);
    	links_vec.clear();
    	link = out__file(link,"_links");
    }

    if(str_find(link,"/"))
    {
    	sys("echo "+link+"> processing\\temp_link.txt");
    	link = "processing\\temp_link.txt";
    }

	

	if(!last)
	if(!exists(link) || find_in_text(link,"********")=="")
	{
		if(!exists(link))	msg_pr(link+" DOESN'T EXIST.");
		else    	msg_pr(link+" DOESN'T have ******** links.");
		mdl_exit();
		exit.exit_now();
	}

    s = find_in_text(link,"https://");

	if(str_find_count(s,"https://")<=2 && str_find_count(s,"https://")!=0)
	{
        ifstream text(link);

		if(!exists("sources\\tags.txt"))		sys("type NUL > sources\\tags.txt");
	    if(!exists("processing\\tags.txt"))		sys("type NUL > processing\\tags.txt");

	    CopyFileA("sources\\tags.txt","sources\\tags_temp.txt",0);

	    vector<string> source_tags_vec;

	    string paths_arr_const[2] = { "sources\\tags_temp.txt", "processing\\tags.txt"};
	    text_merger(paths_arr_const,2,source_tags_vec);

	    int lines_tags=0;

	    string * tags_arr = vector_2_string_array(source_tags_vec,lines_tags);	
	    source_tags_vec.clear();

	    for(int i=0; i<lines_tags; i++)
		tags_arr[i] = get_column(tags_arr[i],15) + "/" + erase_rfind(get_column(tags_arr[i],10),"/") + "|" + tags_arr[i];

		psv_sorter(tags_arr,lines_tags,1);

		del_file("sources\\tags_temp.txt");
		del_file("processing\\new_songs.txt");
		del_file("processing\\new_albums.txt");
		del_file("processing\\new_playlists.txt");
		del_file("processing\\new_lyrics.txt");

		ofstream new_songs, new_albums, new_playists;// new_additions("processing\\new_additions.txt");
		int fgfgf = 0, gh=0, jh=0, mh=0, op=0;

		vector<string> processing_tags_vec, lyrics_vec, album_vec, na;
		int ver=0;
		string safe;
		int kcc=1;
		while(!text.eof())
		{
			getline(text,s);
			if(s.length()==0)	continue;


            if(!other && str_find(s,"|"))
            {
                if(!str_find(get_column(s,2),"/song/")) continue;

                source_url.push_back(s);
                s = get_column(s,2);
            }

			safe = s;
			if(json_source=="********")		temp = "********" + s;
			else if(json_source=="********")	temp = "********" + s;
			else						temp = "********" + s;
			s = erase_rfind(s,"/");

			if(json_source=="********" && str_find(temp,"/playlist/"))
			{
				if(!ver)
				{
					msg_pr("\n");
					cout<<"\n\t*******************************\n";
				}

				ver=1;
				continue;
			}

			if(re_down_pl && str_find(temp,"/playlist/"))
			{
				del_file("sources\\json\\"+s+".json");
				if(jh)	new_playists<<endl;
				else
				{
					new_playists.open("processing\\new_playlists.txt");
					jh = 1;
				}
				new_playists<<temp<<"\n\tout=sources\\json\\"<<s<<".json"<<"\n\ttimeout="<<json_timeout<<"\n\tmax-tries="<<json_playlist_retries;
				na.push_back(safe);
				continue;
			}
			else if(str_find(temp,"/playlist/"))
			{
				if(exists("sources\\json\\"+s+".json"))
				{
					if(find_in_text("sources\\json\\"+s+".json","/playlist/")!="") continue;
					else del_file("sources\\json\\"+s+".json");
				}
			}

			if(str_find(temp,"/song/"))
			if(binary_search_string(tags_arr,0,lines_tags,play_list_name+"/"+s,1)>=0 && !newrelaunch)	continue;

			if(ss_json2tags("sources\\json\\"+s+".json",processing_tags_vec,lyrics_vec,replace_,play_list_name)) continue;
            else del_file("sources\\json\\"+s+".json");

			if(!(str_find(temp,"/album/") && json_source=="********"))
			na.push_back(safe);

			if(str_find(temp,"/song/"))
			{
				if(fgfgf)	new_songs<<endl;
				else
				{
					new_songs.open("processing\\new_songs.txt");
					fgfgf = 1;
				}
				
				new_songs<<temp<<"\n\tout=sources\\json\\"<<s<<".json"<<"\n\ttimeout="<<json_timeout<<"\n\tmax-tries="<<json_song_retries;
			}
			else if(str_find(temp,"/album/"))
			{
				if(gh)	new_albums<<endl;
				else
				{
					new_albums.open("processing\\new_albums.txt");
					gh = 1;
				}
				if(json_source!="********")	new_albums<<temp<<"\n\tout=sources\\json\\"<<s<<".json"<<"\n\ttimeout="<<json_timeout<<"\n\tmax-tries="<<json_album_retries;
				else
				{
					if(!exists("sources\\album_bodies\\"+s))
					dl_status = system(("sources\\tools\\aria2c.exe -q -t 600 "+safe+" -o sources\\album_bodies\\"+s).c_str());

					ss_ale("sources\\album_bodies\\"+s,album_vec);
				}
			}
			else
			{
				if(jh)	new_playists<<endl;
				else
				{
					new_playists.open("processing\\new_playlists.txt");
					jh = 1;
				}
				new_playists<<temp<<"\n\tout=sources\\json\\"<<s<<".json"<<"\n\ttimeout="<<json_timeout<<"\n\tmax-tries="<<json_playlist_retries;
			}
		}
		
		vec_size = album_vec.size();
		if(vec_size)
		{
			for(int i=0; i<vec_size; i++)
			{
				if(fgfgf)	new_songs<<endl;
				else
				{
					new_songs.open("processing\\new_songs.txt");
					fgfgf = 1;
				}
				s = album_vec[i];
				temp = "********" + s;
				new_songs<<temp<<"\n\tout=sources\\json\\"<<erase_rfind(s,"/")<<".json"<<"\n\ttimeout="<<json_timeout<<"\n\tmax-tries="<<json_song_retries;

				na.push_back(safe);
			}
		}

		if(fgfgf) 	new_songs.close();
		if(gh)		new_albums.close();
		if(jh)		new_playists.close();
		text.close();

		if(na.size()>0 && json_source=="********")
		{
			if(!exists("sources\\python"))
			{
				s = pipe_open("python --version");
				s = erase_find_found_to_end(s," ");
				if(s.length()!=6)
				{
					mdl_exit();
					exit.exit_now("\tPython is not installed. json_source as ******** only works with python environment.");
				}
				else
				{
                    int x, y;
                    current_console_cursor(x,y);
					msg_pr("******** Installing requirements for json-source as ******** ******** ");

                    create_process_wo__ prcs;
                    int sf = prcs.create("pip3 install -r \""+sd+"********");

                    if(sf)
                    {
                        mdl_exit();
                        exit.exit_now("******** Failed... Please Try Again ********");
                    }
                    else
                    {
                        sf = prcs.create("pip3 install -r \""+sd+"********");
                        if(sf || str_find_count(prcs.std_out,"Requirement already satisfied:")<16)
                        {
                            mdl_exit();
                            exit.exit_now("******** Failed... Please Try Again ********");
                        }
                        else 
                        {
                            HANDLE hfile = CreateFileA("sources\\python", GENERIC_READ, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, NULL);
                            CloseHandle(hfile);
                        }
                    }

                    msg_pr("******** Installation Successfull ********\n");
                    Sleep(2000);
                    erase_console(x,y);
				}
			}

			pid_ssapi = shell_execute("python","********","",0);
            if(pid_ssapi==-1)
            {
                mdl_exit();
                exit.exit_now("\tFailed... Error occurred with python environment or ********");
            }
			Sleep(5000);
		}

		int dl_status1, dl_status2, dl_status3;

		int lnp  = lines_of_text("processing\\new_playlists.txt");
		int lna  = lines_of_text("processing\\new_albums.txt");
		int lns  = lines_of_text("processing\\new_songs.txt");
		int lnad = na.size();

		current_console_cursor(start_x,start_y);
		if(lnp>0 && !pbjp && !qjp)	msg_pr("Downloading Playlist jsons...\n");
		if(lnp>0 && pbjp)
		{
			pb.task = "\tDownloading Playlist jsons : ";
			apbps.launch();
		}

		if(lnp>0)
			dl_status1 = system(("sources\\tools\\aria2c.exe -c -j "+i2s(json_playlist_concurrecncy)+" -i processing\\new_playlists.txt"+params_json_playlists).c_str());

		if(lnp>0 && pbjp)	apbps.terminate();	pp.resume();

		if(lnp>0 && !pbjp && !qjp && qdynamic)	console_pointer("etls "+to_string(start_x));

		current_console_cursor(start_x,start_y);
		if(lna>0 && !pbja && !qja)	msg_pr("Downloading Album jsons...\n");
		if(lna>0 && pbja)
		{
			pb.task = "\tDownloading Album jsons : ";
			apbps.relaunch();
		}

		if(lna>0)
			dl_status2 = system(("sources\\tools\\aria2c.exe -c -j "+i2s(json_album_concurrecncy)+" -i processing\\new_albums.txt"+params_json_albums).c_str());

		if(lna>0 && pbja)	apbps.terminate();	pp.resume();
		if(lna>0 && !pbja && !qja && qdynamic)	console_pointer("etls "+to_string(start_x));

		current_console_cursor(start_x,start_y);
		if(lns>0 && !pbjs && !qjs)	msg_pr("Downloading Song jsons...\n");
		if(lns>0 && pbjs)
		{
			pb.task = "\tDownloading Song jsons : ";
			apbps.relaunch();
		}

		if(lns>0)
			dl_status3 = system(("sources\\tools\\aria2c.exe -c -j "+i2s(json_song_concurrency)+" -i processing\\new_songs.txt"+params_json_songs).c_str());

		if(lns>0 && pbjs)	apbps.terminate(); pp.resume();
		if(lns>0 && !pbjs && !qjs && qdynamic)	console_pointer("etls "+to_string(start_x));
		if(lnad>0 && json_source=="********")
		pipe_open("taskkill /pid "+to_string(pid_ssapi));

		lyrics_vec.clear();
		
		vector<string> new_tags_vec;
		int nas = na.size();
		if(nas)
		for(int i=0; i<nas; i++)
		ss_json2tags("sources\\json\\"+erase_rfind(na[i],"/")+".json",new_tags_vec,lyrics_vec,replace_,play_list_name);

		vec_size = new_tags_vec.size();
		for(int i=0; i<vec_size; i++)
		processing_tags_vec.push_back(new_tags_vec[i]);

		int lines_new_tags = 0;
		string * new_tags_arr = vector_2_string_array(new_tags_vec,lines_new_tags);

		for(int i=0; i<lines_new_tags; i++)
		new_tags_arr[i] = new_tags_arr[i] + "|" + erase_rfind(get_column(new_tags_arr[i],10),"/");

		psv_sorter(new_tags_arr,0,lines_new_tags,16);

		vector<string> corrupted_lyrics_vec;

		vec_size = lyrics_vec.size();
		for(int i=0; i<vec_size; i++)
		{	
			int f=-1;
            string ly = get_column(lyrics_vec[i],2);
            if(ly=="") continue;

            write_to_text("sources\\lyrics\\"+get_column(lyrics_vec[i],1)+".txt",ly,0);
		}
		lyrics_vec.clear();

		if(json_source=="********" && new_tags_vec.size()>0)
		{
			int vec_size=new_tags_vec.size();
			for(int i=0; i<vec_size; i++)
			corrupted_lyrics_vec.push_back(new_tags_vec[i]);
		}

		vec_size = corrupted_lyrics_vec.size();
		if(vec_size>0)
		{
			ofstream new_lyrics("processing\\new_lyrics.txt");

			for(int i=0; i<vec_size; i++)
			if(!exists("sources\\lyrics\\"+erase_rfind(get_column(corrupted_lyrics_vec[i],10),"/")+".txt"))
			new_lyrics<<"********"+get_column(corrupted_lyrics_vec[i],5)<<"\n\tout=sources\\lyrics\\"<<erase_rfind(get_column(corrupted_lyrics_vec[i],10),"/")<<".json"<<"\n\ttimeout="<<json_timeout<<"\n\tmax-tries="<<json_lyrics_retries<<endl;
			new_lyrics.close();

			current_console_cursor(start_x,start_y);
			if(!pbjl && !qjl)	msg_pr("Downloading Lyrics jsons...\n");
			if(pbjl)
			{
				pb.task = "\tDownloading Lyrics jsons : ";
				apbps.relaunch();
			}
			dl_status = system(("sources\\tools\\aria2c.exe -c -j "+i2s(json_lyrics_concurrency)+" -i processing\\new_lyrics.txt"+params_json_lyrics).c_str());

			if(pbjl)	apbps.terminate(); pp.resume();
			if(!pbjl && !qjl && qdynamic)	console_pointer("etls "+to_string(start_x));
			string lyrics_file;
			for(int i=0; i<vec_size; i++)
			{
				lyrics_file = "sources\\lyrics\\"+erase_rfind(get_column(corrupted_lyrics_vec[i],10),"/");
				
				s = "";
				if(exists(lyrics_file+".json"))	s = ss_lyrics(lyrics_file+".json");
				if(s!="")	write_to_text(lyrics_file+".txt",s,0);
			}
		}

		//Sleep(3000);
		apbps.terminate();

        sort(source_url.begin(), source_url.end(), [](auto & s1, auto & s2) { return get_column(s1,2) < get_column(s2,2); });
        int vs = source_url.size();

        fstream fs = append_file("processing\\tags.txt");

        vec_size = processing_tags_vec.size();

        for(int i=0; i<vec_size; i++)
        {
            if(i) fs<<endl;

            if(!other && vs>=0)
            {
                found = binary_search_string_vec(source_url,0,vs,get_column(processing_tags_vec[i],10),2);
                if(found!=-1)
                {
                    s = "|"+get_column(source_url[found],1);
                    source_url.erase(source_url.begin()+found);
                    vs--;
                }
            }
            fs<<processing_tags_vec[i]<<s;
            fs.flush();
        }
        fs.close();

        source_url.clear();

		int hh=0;
		if(nas)
		{
			for(int i=0; i<nas; i++)
			{
				s = na[i];
				if(s.length()==0)	continue;

				if(find_in_text("sources\\json\\"+erase_rfind(s,"/")+".json","\"id\"")=="")
				{
					if(!hh)		msg_pr("Jsonify failed for : \n");
					hh=1;	
					cout<<s<<endl;
				}
			}
		}

		if(lines_tags)		delete [] tags_arr;
		na.clear();
	}
	else if(str_find_count(s,"|")>4)
	{
		if(exists("processing\\tags.txt"))
		{
			vector<string> merged_vec;
			string paths_arr_const[2] = { "processing\\tags.txt", link };
			text_merger(paths_arr_const,2,merged_vec);

			ofstream o("processing\\tags_temp.txt");
			int ff=0, vec_size = merged_vec.size();
			for(int i=0; i<vec_size; i++)
			{
				if(merged_vec[i].length()==0)	continue;
				if(ff)	o<<endl;
				else	ff=1;
				o<<merged_vec[i];
			}
			o.close();
			merged_vec.clear();

			MoveFileExA("processing\\tags_temp.txt","processing\\tags.txt",1|2);
		}
		else	CopyFileA(link.c_str(),"processing\\tags.txt",0);
	}
}
basic_tags_done=1;

if(!lines_of_text_0("processing\\tags.txt"))	download_processing = 0;

if(download_processing)
{
	del_file("processing\\dl.txt");
	del_file("processing\\dl_covers.txt");
	del_file("processing\\dl_session.txt");
	del_file("processing\\tags_dlr.txt");

	string zstr = text_to_string("processing\\tags.txt");
	int zlen = zstr.length();

	while(zlen && zstr[0]=='\n')
	{
		zstr.erase(0,1);
		zlen--;
	}	
	
	while(zlen && zstr[zlen-1]=='\n')
	{
		zstr.erase(zlen-1,1);
		zlen--;
	}

	write_to_text("processing\\tags.txt",zstr,0);

	if(remove_duplicates)
	{
		vector<string> ptv;
		text_to_vector("processing\\tags.txt", ptv);
		int lines = 0;

		string * stlink  = text_to_string_array("sources\\tags.txt", &lines, 10);
		string * stpl	 = text_to_string_array("sources\\tags.txt", &lines, 15);
        
        vector<string> st__var, st__var_temp;
        text_to_vector("sources\\tags.txt",st__var_temp);

        int vs_st__var = st__var_temp.size();
        string pln, temp_lvar;
        for(int i=0; i<vs_st__var; i++)
        {
            temp_lvar = get_column(st__var_temp[i],14);

            if(!strong) pln = get_column(st__var_temp[i],15)+"/";
            else pln = "";

            if(temp_lvar!="") st__var.push_back(pln + temp_lvar);
        }
        st__var_temp.clear();

		for(int i=0; i<lines; i++)
		{
            if(!strong) pln = stpl[i] + "/";
            else pln="";

			stlink[i]  = pln + erase_rfind(stlink[i],"/");
		}

		sort(stlink,stlink+lines);
		sort(st__var.begin(),st__var.end());

		int vs = ptv.size();


        if(!strong)
        {
            for(int i=0; i<vs; i++)
            ptv[i] =  get_column(ptv[i],15) +"/"+erase_rfind(get_column(ptv[i],10),"/") + "|" + ptv[i];

            psv_dlr(ptv,1);
            vs = ptv.size();

            for(int i=0; i<vs; i++)
            ptv[i] = erase_find(ptv[i],"|");


            vector<string> vcna, vca;

            for(int i=0; i<vs; i++)
            {
                if(get_column(ptv[i],14)=="") vcna.push_back(ptv[i]);
                else vca.push_back(vca[i]);
            }

            if(vca.size())
            {
                ptv.clear();
                vs = vca.size();

                for(int i=0; i<vs; i++)
                ptv.push_back(vca[i]);

                vca.clear();

                for(int i=0; i<vs; i++)
                ptv[i] = get_column(ptv[i],15) +"/"+get_column(ptv[i],14) + "|" + ptv[i];
                psv_dlr(ptv,1);

                vs = ptv.size();
                for(int i=0; i<vs; i++)
                ptv[i] = erase_find(ptv[i],"|");

                vs = vcna.size();
                for(int i=0; i<vs; i++)
                ptv.push_back(vcna[i]);
            }
            
            vcna.clear();
        }
        else
        {
            psv_dlr(ptv,10);
            psv_dlr(ptv,14);
        }

		int ff=0;
		string s;
		vector<string> dv, dvv;
		if(scan_earlier && newrelaunch && dir_size(dd))
		{
            vector<string> v;
            dir_content(dd,&v);
            int vs = v.size();

			stringstream ss;
			string file, pl;
            pb.task = "\tScanning Earlier Downloads : ";
            pb.total_count=vs;
            pb.current_count=0;
			for(int i=0; i<vs; i++)
			{
				file = v[i];
				if(file=="") continue;

                if(pb.execute && !qsed)
                {
                    pb.current_count++;
                    pb.new_line = "\t" + file;
                    pb.update();
                }

				pp.resume();

                if(!strong) s = pipe_open("sources\\tools\\kid3\\kid3-cli.exe -c \"get playlist\" -c \"get wwwaudiofile\" -c \"get ********\" \""+file+"\"");
                else        s = pipe_open("sources\\tools\\kid3\\kid3-cli.exe -c \"get wwwaudiofile\" -c \"get ********\" \""+file+"\"");

				if(!strong && str_find(s,"https://"))
				{
                    pl="";
					ss = stringstream(s);
					getline(ss,s);
                    if(!str_find(s,"https://"))
                    {
                        pl = s;
                        getline(ss,s);
                        dv.push_back(pl+"/"+erase_rfind(s,"/"));
                    }
                    else dv.push_back(pl+"/"+erase_rfind(s,"/"));
					
                    if(!ss.eof())
                    {
                        getline(ss,s);
                        if(s.length()>5) dvv.push_back(pl+"/"+s);
                    }
				}
                else if(strong && str_find(s,"https://"))
                {
                    ss = stringstream(s);
                    getline(ss,s);
                    dv.push_back(erase_rfind(s,"/"));
                    
                    getline(ss,s);
                    if(s.length()>5) dvv.push_back(s);
                }
				if(i==vs-1 && pb.execute && !qsed) pb.update(1);
			}
			pb.terminate();
		}

		int dvs = dv.size(), dvvs=dvv.size();
		sort(dv.begin(),dv.end());
		sort(dvv.begin(),dvv.end());
		ofstream out("processing\\tags.txt");
		string t;
        vs_st__var = st__var.size();
        vs = ptv.size();

		for(int i=0; i<vs; i++)
		{
			if(!newrelaunch)
			{
                if(!strong) t = get_column(ptv[i],15)+"/"+erase_rfind(get_column(ptv[i],10),"/");
                else t = erase_rfind(get_column(ptv[i],10),"/");

				if(binary_search_string(stlink,0,lines,t)!=-1) continue;
				else if(get_column(ptv[i],14)!="")
				{
					s = get_column(ptv[i],14);
                    if(!strong) t = get_column(ptv[i],15)+"/"+s;
                    else t = s;

					if(binary_search_string_vec(st__var,0,vs_st__var,t)!=-1)	continue;				
				}
			}
			else if(dvs)
			{
                if(!strong) t = get_column(ptv[i],15)+"/"+erase_rfind(get_column(ptv[i],10),"/");
                else t = erase_rfind(get_column(ptv[i],10),"/");

				if(binary_search(dv.begin(),dv.end(),t)) continue;
				
                if(!strong) t = get_column(ptv[i],15)+"/"+get_column(ptv[i],14);
                else t = get_column(ptv[i],14);
                if(dvvs && get_column(ptv[i],14)!="" && binary_search(dvv.begin(),dvv.end(),t)) continue;
			}

			if(ff)	out<<endl;
			else  	ff=1;
			out<<ptv[i];
		}

		out.close();
		ptv.clear();
		dv.clear();
		dvv.clear();
        st__var.clear();
		delete [] stlink;
		delete [] stpl;
	}

	int lines_source_tags = lines_of_text("sources\\tags.txt");
	if(lines_source_tags==-1)	lines_source_tags = 0;
	string * source_tags_arr;

	if(lines_source_tags)
	{
		source_tags_arr = text_to_string_array("sources\\tags.txt",&lines_source_tags);

		for(int i=0; i<lines_source_tags; i++)
		source_tags_arr[i] = erase_rfind(get_column(source_tags_arr[i],10),"/") + "|" + source_tags_arr[i];

		psv_sorter(source_tags_arr,lines_source_tags,1);
	}

	if(!basic_tags_done)
	{
		ifstream text("processing\\tags.txt");
		if(!text)
		{
			mdl_exit();
			exit.exit_now();
		}

        md_if("sources\\basic_tag_bodies");
		ofstream basic_tags_dl("processing\\basic_tags_dl.txt");
		int gh=0;

		vector<string> basic_tags_found_paths_vec;

		while(!text.eof())
		{
			getline(text,s);
			if(s.length()==0)	continue;
			if(lines_source_tags)	if(binary_search_string(source_tags_arr,0,lines_source_tags,erase_rfind(get_column(s,10),"/"),1)!=-1)	continue;

			basic_tags_found_paths_vec.push_back("sources\\basic_tag_bodies\\"+erase_rfind(get_column(s,10),"/"));

			if(exists("sources\\basic_tag_bodies\\"+erase_rfind(get_column(s,10),"/")))	continue;

			if(gh)	basic_tags_dl<<endl;
			else	gh = 1;
			basic_tags_dl<<get_column(s,10)<<"\n\tout=sources\\basic_tag_bodies\\"+erase_rfind(get_column(s,10),"/")<<"\n\tmax-tries="<<i2s(basic_tag_retries);
		}
		text.close();
		basic_tags_dl.close();

		current_console_cursor(start_x,start_y);
		if(lines_of_text("processing\\basic_tags_dl.txt")>0 && !pbb && !qb)	msg_pr("Downloading Basic Tag Bodies...\n");
		if(lines_of_text("processing\\basic_tags_dl.txt")>0 && pbb)
		{
			pb.task = "\tDownloading Basic Tag Bodies : ";
			apbps.relaunch();
		}

		if(lines_of_text("processing\\basic_tags_dl.txt")>0)
		dl_status = system(("sources\\tools\\aria2c.exe -c -j "+i2s(basic_tag_concurrency)+" -i processing\\basic_tags_dl.txt --save-session=processing\\basic_tags_dl_session.txt"+params_basic_tags).c_str());
		

		if(lines_of_text("processing\\basic_tags_dl.txt")>0 && pbb)	apbps.terminate();  pp.resume();

		if(lines_of_text("processing\\basic_tags_dl.txt")>0 && !pbb && !qb && qdynamic)	console_pointer("etls "+to_string(start_x));

		current_console_cursor(start_x,start_y);
		if(!dl_status && lines_of_text("processing\\basic_tags_dl_session.txt")>0 && !pbb && !qb)	msg_pr("Retrying Failed Basic Tag Bodies...\n");
		if(!dl_status && lines_of_text("processing\\basic_tags_dl_session.txt")>0 && pbb)
		{
			pb.task = "\tRetrying Failed Basic Tag Bodies : ";
			apbps.relaunch();
		}


		if(!dl_status && lines_of_text("processing\\basic_tags_dl_session.txt")>0)	dl_status = system(("sources\\tools\\aria2c.exe -c -j "+i2s(basic_tag_concurrency)+" -i processing\\basic_tags_dl_session.txt"+params_basic_tags).c_str());

		if(!dl_status && lines_of_text("processing\\basic_tags_dl_session.txt")>0 && pbb)
			apbps.terminate();	pp.resume();
		if(!dl_status && lines_of_text("processing\\basic_tags_dl_session.txt")>0 && !pbb && !qb && qdynamic)	console_pointer("etls "+to_string(start_x));

		vector<string> basic_tags_vec;
		int vec_size = basic_tags_found_paths_vec.size();
		for(i=0; i<vec_size; i++)
		{
			s = ss_bte(basic_tags_found_paths_vec[i]);
			if(s!="")	basic_tags_vec.push_back(s);
		}

		int lines_basic_tags = 0;
		string * basic_tags_arr = vector_2_string_array(basic_tags_vec,lines_basic_tags);
		basic_tags_vec.clear();

		vector<string> ptv;
		text_to_vector("processing\\tags.txt",ptv);

		vec_size = ptv.size();

		for(i=0; i<vec_size; i++)
		ptv[i] = ss_btr(basic_tags_arr,lines_basic_tags,ptv[i]);

		vector_to_text("processing\\tags.txt",ptv,0);
		ptv.clear();

		if(lines_basic_tags)	delete [] basic_tags_arr;
		lines_basic_tags = 0;
	}


	ofstream dl_covers("processing\\dl_covers.txt");
	ofstream dl("processing\\dl.txt");

	int fgf = 0, cov=0;

	if(dir_size(ddt))		sys("dir /s /b /a-d \""+ddt+"\" >> processing\\download_paths.txt");

	vector<string> covers_vec;
	int exist, aria;
	ifstream text("processing\\tags.txt");
	while(!text.eof())
	{
		getline(text,s);
		if(s.length()==0)	continue;

		exist = exists("sources\\covers\\"+erase_rfind((get_column(s,6)),"/"));
		aria  = exists("sources\\covers\\"+erase_rfind((get_column(s,6)),"/")+".aria2");

		if((!exist || (exist && aria)) && !binary_search(covers_vec.begin(),covers_vec.end(),get_column(s,6)))
		{
			covers_vec.push_back(get_column(s,6));
			sort(covers_vec.begin(),covers_vec.end());
			if(cov)		dl_covers<<endl;
			else		cov = 1;
			if(proxy)	dl_covers<<get_column(s,6).replace(0,8,"********")<<"\n\tout=sources\\covers\\"<<erase_rfind(get_column(s,6),"/")<<"\n\tmax-tries="<<cover_retries;
			else		dl_covers<<get_column(s,6)<<"\n\tout=sources\\covers\\"<<erase_rfind(get_column(s,6),"/")<<"\n\tmax-tries="<<cover_retries;
		}

		if(playlists && ""!=get_column(s,15))
		{	
			exist = exists(ddt+"playlists\\"+get_column(s,15)+"\\"+erase_rfind(get_column(s,8),"/"));
			aria  = exists(ddt+"playlists\\"+get_column(s,15)+"\\"+erase_rfind(get_column(s,8),"/")+".aria2");
			if(!exist || (exist && aria))
			{
				if(fgf)			dl<<endl;
				else			fgf = 1;
				string pld = "playlists\\"+get_column(s,15);
				if((ddt+pld).length()>220)
				{
					if(!pldf)
					{
						Sleep(1000);
						if(!str_find(get_first_line("sources\\logs\\download_errors.log"),"Failed to download songs listed below because playlist name was too long"))
						write_to_text("sources\\logs\\download_errors.log","Failed to download songs listed below because playlist name was too long");

						write_to_text("sources\\logs\\download_errors.log","\n\t******* "+time()+" *******\n");
					}
					pldf=1;

					write_to_text("sources\\logs\\download_errors.log",get_column(s,10)+"|"+ddt+pld);
					continue;
				}

				if(proxy)		dl<<get_column(s,8).replace(0,8,"********")<<"\n\tout=playlists\\"<<get_column(s,15)<<"\\"<<erase_rfind(get_column(s,8),"/")<<"\n\tmax-tries="<<song_retries;
				else			dl<<get_column(s,8)<<"\n\tout=playlists\\"<<get_column(s,15)<<"\\"<<erase_rfind(get_column(s,8),"/")<<"\n\tmax-tries="<<song_retries;
				continue;
			}
		}
		else
		{	
			exist = exists(ddt+erase_rfind(get_column(s,8),"/"));
			aria  = exists(ddt+erase_rfind(get_column(s,8),"/")+".aria2");
			if(!exist || (exist && aria))
			{
				if(fgf)			dl<<endl;
				else			fgf = 1;
				if(proxy)		dl<<get_column(s,8).replace(0,8,"********")<<"\n\tout="<<erase_rfind(get_column(s,8),"/")<<"\n\tmax-tries="<<song_retries;
				else			dl<<get_column(s,8)<<"\n\tout="<<erase_rfind(get_column(s,8),"/")<<"\n\tmax-tries="<<song_retries;
			}
		}
	}
	dl.close();
	dl_covers.close();

	current_console_cursor(start_x,start_y);
	if(lines_of_text("processing\\dl_covers.txt")>0 && !pbc && !qc)	msg_pr("Downloading Album Covers...\n");
	if(lines_of_text("processing\\dl_covers.txt")>0 && pbc)
	{
		pb.task = "\tDownloading Album Covers : ";
		apbps.relaunch();
	}

	if(lines_of_text("processing\\dl_covers.txt")>0)
	dl_status = system(("sources\\tools\\aria2c.exe -c -j "+i2s(cover_concurrency)+" -i processing\\dl_covers.txt --save-session=processing\\dl_covers_session.txt"+params_covers).c_str());
	
	if(lines_of_text("processing\\dl_covers.txt")>0 && pbc)		apbps.terminate();	pp.resume();

	if(lines_of_text("processing\\dl_covers.txt")>0 && !pbc && !qc && qdynamic)	console_pointer("etls "+to_string(start_x));

	current_console_cursor(start_x,start_y);
	if(lines_of_text("processing\\dl_covers_session.txt")>0 && !pbc && !qc)	msg_pr("Retrying Failed Album Covers...\n");
	if(lines_of_text("processing\\dl_covers_session.txt")>0 && pbc)
	{
		pb.task = "\tRetrying Failed Album Covers : ";
		apbps.relaunch();
	}


	if(!dl_status && lines_of_text("processing\\dl_covers_session.txt")>0)
		dl_status = system(("sources\\tools\\aria2c.exe -c -j "+i2s(cover_concurrency)+" -i processing\\dl_covers_session.txt"+params_covers).c_str());

	if(lines_of_text("processing\\dl_covers_session.txt")>0 && pbc)		apbps.terminate(); pp.resume();
	if(lines_of_text("processing\\dl_covers_session.txt")>0 && !pbc && !qc && qdynamic)	console_pointer("etls "+to_string(start_x));

	current_console_cursor(start_x,start_y);
	if(lines_of_text("processing\\dl.txt")>0 && !pbd && !qd)	msg_pr("Downloading Songs...\n");
	if(lines_of_text("processing\\dl.txt")>0 && pbd)
	{
		pb.task = "\tDownloading Songs : ";
		apbps.relaunch();
	}
	if(!lines_of_text_0("processing\\dl.txt"))	download_processing = 0;

	if(lines_of_text("processing\\dl.txt")>0)
	dl_status = system(("sources\\tools\\aria2c.exe -c -s 16 -x 16 -k 1M -d \""+dda+"\" -j "+i2s(song_concurrency)+" -i processing\\dl.txt --save-session=processing\\dl_session.txt"+params_song).c_str());

	if(lines_of_text("processing\\dl.txt")>0 && pbd)	apbps.terminate(); pp.resume();
	if(lines_of_text("processing\\dl.txt")>0 && !pbd && !qd && qdynamic)	console_pointer("etls "+to_string(start_x));

	current_console_cursor(start_x,start_y);
	if(lines_of_text("processing\\dl_session.txt")>0 && !pbd && !qd)		msg_pr("Retrying Failed Songs...\n");
	if(lines_of_text("processing\\dl_session.txt")>0 && pbd)
	{
		pb.task = "\tRetrying Failed Songs : ";
		apbps.relaunch();
	}

	if(!dl_status && lines_of_text("processing\\dl_session.txt")>0)
	 dl_status = system(("sources\\tools\\aria2c.exe -c -s 16 -x 16 -k 1M -d \""+dda+"\" -j "+i2s(song_concurrency)+" -i processing\\dl_session.txt"+params_song).c_str());

	if(lines_of_text("processing\\dl_session.txt")>0 && pbd)	apbps.terminate();
	if(lines_of_text("processing\\dl_session.txt")>0 && !pbd && !qd && qdynamic)	console_pointer("etls "+to_string(start_x));

	if(lines_source_tags)	delete [] source_tags_arr;
	del_file("processing\\dl.txt");
	del_file("processing\\dl_covers.txt");
	del_file("processing\\dl_session.txt");
	del_file("processing\\tags_dlr.txt");
}
apbps.terminate();
pb.set_len=0;
pp.delayed_message=0;
pb.extra_info="";
pp.resume();

if(tag_processing && lines_of_text_0("processing\\tags.txt"))
{	
	int lines_tags = 0;

	string * tags_arr = text_to_string_array("processing\\tags.txt", &lines_tags);
    vector<string> tagsv;
    text_to_vector("sources\\tags.txt",tagsv);

	if(lines_tags+tagsv.size()<1)
	{
		mdl_exit();
		exit.exit_now();
	}

	psv_sorter(tags_arr,lines_tags,1);


if(dir_size(ddt))
{
    
    sort(tagsv.begin(),tagsv.end());
    sort(tags_arr,tags_arr+lines_tags);


	sys("dir /s /b /a-d \""+ddt+"\" > processing\\download_paths.txt");
	int ldp = lines_of_text_0("processing\\download_paths.txt");
	
	pb.task = "\tTagging : ";

	vector<string> v, fn;

    string pln;
    int vs = tagsv.size();
	ifstream tt("processing\\download_paths.txt");
    string tmp;

	while(!tt.eof())
	{
		getline(tt,s);
		if(str_find(s,".aria2"))
		{
			del_file(s);
			del_file(erase_rfind(s,".aria2"));
			continue;
		}

        t = s;

        if(str_find_count(s,".",str_find_index(s,"\\",s.length()-1,1)+1)>1)
        {
            t = erase_rfind_found_to_end(t,".");
            t = erase_rfind_found_to_end(t,".") + ext_ex(s);
        }

        pln="";

        if(str_find(s,ddt+"playlists\\"))
        {
            pln = s;
            pln.erase(0,(ddt+"playlists\\").length());
            pln = erase_find_found_to_end(pln,"\\");
        }

		found = binary_search_string(tags_arr,0,lines_tags,ext_fnwe(t),1);
        if(found!=-1) tmp = tags_arr[found];
        else
        {
            found = binary_search_string_vec(tagsv,0,vs,ext_fnwe(t),1);
            if(found!=-1) tmp = tagsv[found];
        }

 		if(found!=-1)
 		{
 			pb.total_count++;
 			v.push_back(write_column(tmp,15,pln));
 			fn.push_back(s);
 		}
	}

	tt.close();
    tagsv.clear();

	pb.thread=0;
	int f, k=0;
	int fhy=0;
	vec_size = v.size();
	vector<int> field;

	string format = tag_script;
	format = replace_string_all(format,"*","");



	if(renaming)
	{
		if(str_find(format,"|directory|") || str_find(format,"|filename|") || str_find(format,"|extension|") || str_find(format,"|playlist|"))
		{
			msg_pr("renaming-format should not contain any of |directory|, |filename|, |extension| and |playlist|\n");
			renaming=0;
		}
		else if(str_find_count(format,"|")%2==1)
		{
			msg_pr("renaming-format is invalid.\n");
			renaming=0;
		}
		else
		{
		    f = str_find_index(format,"|");
		    tag_fields_sn__ tf;
		    while(f!=-1)
		    {
		        string t;
		        i=f+1;

		        while(format[i]!='|')
		        {
		            t.push_back(format[i]);
		            i++;
		        }

		        if(t.length()) 
		        {
		            field.push_back(tf.get_sn(t));
		            format.replace(f,t.length()+2,"*");
		        }
		        else    format.erase(f,2);
		        f = str_find_index(format,"|");
		    }
		}
	}

	string format_ = format;
	string rt, tv, _temp;
	int kh=0;
	if(renaming) pb.task = "\tTagging and Renaming : ";
	string uc;
	int ren_fail=0;
	int ren_log=0;

	for(int i=0; i<vec_size; i++)
	{
		s = fn[i];
		tv = v[i];
		ren_fail=0;


		t = s;
		string t_m4a = t;
		t_m4a = erase_rfind_found_to_end(t_m4a,".") + ".m4a";

		k = str_find_index(t_m4a,"\\"+ddt,t_m4a.length()-1,1);
		t_m4a.replace(k+1,ddt.length(),dd);

		s = erase_rfind(s,"\\");

		if(!qtagging && !renaming)	
		{
			if(!fhy)	fhy = 1;
			pb.current_count++;
			pb.new_line = "\t" + get_column(tv,2) + " - " + get_column(tv,12);
			pb.update();
		}

        pp.resume();

		rt = kid3_tag(t,tv,"sources\\json\\"+erase_rfind(get_column(tv,3),"/")+".json","sources\\tools\\kid3\\kid3-cli.exe",other,remove_words,sd);
		
		md_if(ext_dn(t_m4a));

		if(!renaming) 
		{
			MoveFileExA(t.c_str(),t_m4a.c_str(),1|2);
			if(!i && vec_size==1) only_one_song = t_m4a;
		}
		else
		{
			format = format_;
			kh=0;
			f = str_find_index(format,"*");
			while(f!=-1)
			{
				format.replace(f,1,get_column(rt,field[kh]));
				f = str_find_index(format,"*");
				kh++;
			}

			format += ".m4a";
			format = validate_path(format);
			_temp = format;
			if(playlists && get_column(tv,15)!="")
			{
				_temp = "playlists\\"+validate_path(get_column(tv,15))+"\\"+format;
				format = dd+_temp;
			}
			else format = dd + format;
			format = name_assigner(format);

			if((ext_dn(format)).length()>220 || format.length()>259)
			{
				if(!renaming_failed)
				{
					Sleep(1000);
					if(!str_find(get_first_line("sources\\logs\\renaming_errors.log"),"Failed to rename songs listed below because destination path was too long"))
					write_to_text("sources\\logs\\renaming_errors.log","Failed to rename songs listed below because destination path was too long");
					write_to_text("sources\\logs\\renaming_errors.log","\n\t******* "+time()+" *******\n");
					renaming_failed=1;
				}
				write_to_text("sources\\logs\\renaming_errors.log",t_m4a+"|"+format);
				format = t_m4a;
				_temp  = change_ex(get_column(tv,1),"m4a");
			}
			if(!qtagging)	
			{
				if(!fhy)	fhy = 1;
				pb.current_count++;
				pb.new_line = "\t" + get_column(tv,1) + pb.transition + _temp;
				pb.update();
			}
			MoveFileExA(t.c_str(),format.c_str(),1|2);
			if(!i && vec_size==1) only_one_song = format;
		}
		write_to_text("sources\\tags.txt",tv);
		if(i==vec_size-1 && pb.execute && (!qtagging || !qrenaming)) pb.update(1);
	}
	pb.terminate();
}
	
    int lines_source_tags = 0;
    string * source_tags_arr = text_to_string_array("sources\\tags.txt",&lines_source_tags);

    for(int i=0; i<lines_source_tags; i++)
    source_tags_arr[i] = get_column(source_tags_arr[i],1) +"/"+ get_column(source_tags_arr[i],15);

    sort(source_tags_arr,source_tags_arr+lines_source_tags);

    ofstream out("processing\\tags_temp.txt");
    int fgf = 0;

    for(int i=0; i<lines_tags; i++)
    {
        if(binary_search_string(source_tags_arr,0,lines_source_tags,get_column(tags_arr[i],1)+"/"+get_column(tags_arr[i],15))!=-1)  continue;

        if(fgf)     out<<endl;
        else        fgf = 1;
        out<<tags_arr[i];
    }
    out.close();
    if(lines_tags)          delete [] tags_arr;
    if(lines_source_tags)   delete [] source_tags_arr;
    MoveFileExA("processing\\tags_temp.txt","processing\\tags.txt",1|2); 
}

if(rd!="" && rd[rd.length()-1]!='\\')	rd += "\\";
if(serialize_dir!="" && serialize_dir[serialize_dir.length()-1]!='\\')	serialize_dir += "\\";

pb.thread=pb_thread;

string dir_ren_log_file;
if(rename_processing && exists(rd))
{
	string rd_ = rd;
	if(rd[rd.length()-1]=='\\') rd_.erase(rd.length()-1,1);
	pb.task = "\tRenaming : ";
    progress_bar_param *pbp=NULL;
    if(!qrenaming) pbp = &pb;
	dir_ren_log_file = renamer(rename_script, rd,other_files,pbp,ppp,"sources\\logs\\dir-ren_logs","sources\\tools\\kid3\\kid3-cli.exe", extensions);
}

if(!dir_size(ddt) && exists(ddt))	pipe_open("rd /s /q \""+ddt+"\"");

	pb.terminate("up");
	pp.terminate();

	mdl_exit();

	if(pldf || renaming_failed || dir_ren_log_file!="")
	{
		msg_pr("|");
		if(pldf) 			cout<<"\tSome songs couldn't be downloaded because their playlist names were too long. See sources\\logs\\download_errors.log\n";
		if(renaming_failed) cout<<"\tSome songs couldn't be renamed because their destination paths were too long. See sources\\logs\\renaming_errors\n";
		if(dir_ren_log_file!="")  cout<<"\tSome songs couldn't be renamed by dir-ren because their source or/and destination paths were too long. See \""+dir_ren_log_file<<"\""<<endl;
	}

	if(download_processing && basic_info && argc==1)
	{
		msg_pr("\n\tDownloaded...\tEnter 'y' to open download folder else press enter : ",0);
		getline(cin,s); if (cin.fail()) return 0;
		if(str_find(lower_case_converter(s),"y"))
		{
			if(only_one_song=="")	sys("explorer \""+dd);
			else sys("explorer /select, \""+only_one_song+"\"");
		}
		exit.argc = 2;
		exit.msg="";
		exit.exit_now();
	}
	else if(basic_info && download_processing) exit.exit_now("\tDownloaded...");
	else if(basic_info) exit.exit_now("\tDone...");
}