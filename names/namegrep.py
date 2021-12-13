from urllib.request import urlopen
from bs4 import BeautifulSoup

from random import randint
from time import sleep

namesets = [
  # momj
  "aboriginal", "african", "albanian", "arabic", "armenian", "assamese",
  "australian", "basque", "bengali", "buddhism", "bulgarian", "catalan", "celtic",
  "chechen", "chinese", "christianity", "cornish", "croatian", "czech", "danish",
  "dutch", "egyptian", "english", "estonian", "filipino", "finnish", "french",
  "frisian", "georgian", "german", "greek", "gujarati", "hawaiian", "hebrew",
  "hindi", "hindu", "hungarian", "icelandic", "indian", "indonesian", "irish",
  "islam", "italian", "jainism", "japanese", "judaism", "kannada", "kazakh",
  "korean", "kurdish", "latvian", "lithuanian", "macedonian", "manx", "marathi",
  "mongolian", "native-american", "nepali", "norse", "norwegian", "oriya",
  "pashto", "persian", "polish", "portuguese", "romanian", "russian", "sanskrit",
  "scandinavian", "scottish", "serbian", "shinto", "sikh", "slovak", "swahili",
  "swiss", "tamil", "telugu", "thai", "turkish", "united-states", "urdu",
  "vietnamese", "zoroastrianism",

  # ermehgherd!
  "aboriginal", "african", "albanian", "arabic", "armenian", "assamese",
  "australian", "azerbaijani", "chechen", "chinese", "filipino", "georgian",
  "hebrew", "indian", "indonesian", "japanese", "kazakh", "khmer", "korean",
  "kurdish", "kyrgyz", "mongolian", "nepali", "pakistani", "pashto", "persian",
  "tajik", "tatar", "tibetan", "thai", "turkish", "turkmen", "uyghur", "uzbek",
  "vietnamese", "albanian", "basque", "belarusian", "breton", "bulgarian",
  "catalan", "cornish", "corsican", "croatian", "czech", "danish", "dutch",
  "english", "estonian", "finnish", "french", "frisian", "galician", "german",
  "greek", "hungarian", "icelandic", "irish", "italian", "latvian", "limburgish",
  "lithuanian", "macedonian", "manx", "norwegian", "occitan", "polish",
  "portuguese", "romanian", "russian", "sardinian", "serbian", "scottish",
  "slovak", "slovene", "spanish", "swedish", "ukrainian", "welsh", "yiddish",
  "esperanto", "hawaiian", "maori", "native-american", "astronomy", "history",
  "literature", "popular-culture", "theology", "ancient", "medieval"
  "all-biblical", "mythology"]

print(namesets[0])

def scrape_momj(page):
  html = page.read().decode("utf-8")
  soup = BeautifulSoup(html, 'html.parser')
  tr = soup.find_all("tr")
  tsv = ""
  for i in range(1, len(tr)):
    tds = tr[i].find_all("td")
    if len(tds) == 7:
      tsv += tds[0].text.strip() # name
      tsv += "\t"
      tsv += tds[1].text.strip() # meaning
      tsv += "\t"

      gender = tds[2].text.strip()
      if (gender == "Boy"):
        tsv += "m\n"
      elif (gender == "Girl"):
        tsv += "f\n"
      elif (gender == "Unisex"):
        tsv += "e\n"
      else:
        tsv += "u\n"
  return tsv

def rng_sleep():
  sleep(randint(1,5))

def mom_juntion():
  base_url = "http://www.momjunction.com/baby-names/"

  for name in namesets:
    tsv = "NAMES	MEANING	GENDER\n"
    # first pass
    try:
      url = f"{base_url}{name}"
      print(f"Trying {url}")
      page = urlopen(url)
      tsv += scrape_momj(page)
    except:
      continue

    for i in range(2, 100):
      url = f"{base_url}{name}/page/{i}/"
      print(f"Trying {url}")
      rng_sleep()
      try:
        page = urlopen(url)
        tsv += scrape_momj(page)
      except:
        break

    with open(f"momj/{name}.tsv", 'w', encoding='utf-8') as f:
      f.write(tsv)

mom_juntion()
