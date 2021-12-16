
// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

#include <iostream>
#include <stdio.h>

// ecs36b
#include <time.h>
using namespace std;

int
main(int argc, char *argv[])
{
  if (argc != 2) return -1;

  // open the file
  FILE *jf = fopen(argv[1], "r");
  if (jf == NULL) return -1;

  // calculate the size of the file
  long lSize;
  size_t lresult;

  // obtain file size:
  fseek(jf, 0 , SEEK_END);
  lSize = ftell(jf);
  rewind(jf);

  // allocate memory to contain the whole file:
  char *jf_ptr = (char *) malloc(sizeof(char)*lSize);
  // copy the file into the buffer:
  lresult = fread(jf_ptr, 1, lSize, jf);    
  if (lresult != lSize)
    {
      fputs("Reading error", stderr);
      exit (-3);
    }

  Json::Value myv_message;
  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  std::string errors;

  try
    {
  bool parsingSuccessful = reader->parse
    (jf_ptr, jf_ptr + lSize, &myv_message, &errors);
  delete reader;

  if (!parsingSuccessful) {
    std::cout << "Failed to parse the content JSON, errors:" << std::endl;
    std::cout << errors << std::endl;
  }
    }
  catch(...)
    {
      std::cout << "here\n";
    }

  std::cout << myv_message.toStyledString() << std::endl;
  return 0;
}
