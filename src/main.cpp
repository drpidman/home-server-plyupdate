#include <stdio.h>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>
#include <unistd.h>
#include <pwd.h>
#include <curl/curl.h>



using namespace std;
namespace fs = boost::filesystem;

void DownloadFile(const char *filename, const char *url)
{
    CURL *curl = curl_easy_init();
    if (curl)
    {

        FILE *file = fopen(filename, "wb");
        if (file)
        {
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                cerr << "Erro ao baixar o arquivo:" << curl_easy_strerror(res) << endl;
                fclose(file);
                fs::remove(filename);
            }
            else
            {
                fclose(file);
                cout << "O arquivo foi baixado com sucesso " << filename << endl;
            }
        }
        else
        {
            cerr << "Erro ao criar o arquivo" << endl;
        }

        curl_easy_cleanup(curl);
    }
    else
    {
        cerr << "Erro ao inicializar o curl" << endl;
    }
}

void UpdateLocalPlaylist()
{
    struct passwd *pwd = getpwuid(getuid());

    string homedir = pwd->pw_dir;
    string playlistdir = homedir += "/infra/i.mjh.nz/PlutoTV/";
    string br_m3u8_fname = playlistdir + "br.m3u8";
    string br_xml_fname = playlistdir + "br.xml";

    string m3u8_url = "https://i.mjh.nz/PlutoTV/br.m3u8";
    string xml_url = "https://i.mjh.nz/PlutoTV/br.xml";

    if (!fs::exists(br_m3u8_fname) && !fs::exists(br_xml_fname))
    {
        cout << "Baixando arquivo de https://i.mjh.nz/PlutoTV para: " << playlistdir << endl;
        cout << "Baixando arquivo -> br.m3u8" << endl;
        DownloadFile(br_m3u8_fname.c_str(), m3u8_url.c_str());

        cout << "Baixando arquivo -> br.xml" << endl;
        DownloadFile(br_xml_fname.c_str(), xml_url.c_str());


    }
    else
    {
        cout << "Removendo br.m3u8" << endl;
        fs::remove(br_m3u8_fname.c_str());
        cout << "Removendo br.xml" << endl;
        fs::remove(br_xml_fname.c_str());

        cout << "Baixando novos arquivos" << endl;
        DownloadFile(br_m3u8_fname.c_str(), m3u8_url.c_str());
        DownloadFile(br_xml_fname.c_str(), xml_url.c_str());

    }
}

int main(int argc, char *argv[])
{
    UpdateLocalPlaylist();
}