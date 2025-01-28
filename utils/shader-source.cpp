#include "utils.h"
#include <fstream>
#include <iostream>

std::string Utils::shaderSource(
  const char* path,
  const std::vector<std::string>& prependTexts,
  const std::vector<std::string>& prependFiles
)
{
  std::string source;
  std::vector<std::string> pFiles;
  try {
    source = Utils::readFile(path);

    for (unsigned int i = 0; i < prependFiles.size(); i++) {
      pFiles.push_back(Utils::readFile(prependFiles[i].c_str()));
    }
  } catch (std::ifstream::failure& e) {
    std::cout << "Error loading shader\n" << path << "\n" << e.what() << std::endl;
    return 0;
  }

  if (prependTexts.size() > 0 || prependFiles.size() > 0) {
    std::size_t versionEndPos = 0;
    if (source.rfind("#version", 0) == 0) {
      versionEndPos = source.find('\n') + 1;
    }

    for (unsigned int i = pFiles.size(); i-- > 0;) {
      source.insert(versionEndPos, pFiles[i]);
    }

    for (unsigned int i = prependTexts.size(); i-- > 0;) {
      source.insert(versionEndPos, prependTexts[i]);
    }
  }

  return source;
}
