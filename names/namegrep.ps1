[Net.ServicePointManager]::SecurityProtocol =[Net.SecurityProtocolType]::Tls12

$namesets = @(
  "aboriginal",
  "african",
  "arabic",
  "armenian",
  "azerbaijani",
  "chechen",
  "chinese",
  "filipino",
  "georgian",
  "hebrew",
  "indian",
  "indonesian",
  "japanese",
  "kazakh",
  "khmer",
  "korean",
  "kurdish",
  "kyrgyz",
  "mongolian",
  "nepali",
  "pakistani",
  "pashto",
  "persian",
  "tajik",
  "tatar",
  "tibetan",
  "thai",
  "turkish",
  "turkmen",
  "uyghur",
  "uzbek",
  "vietnamese",
  "albanian",
  "basque",
  "belarusian",
  "breton",
  "bulgarian",
  "catalan",
  "cornish",
  "corsican",
  "croatian",
  "czech",
  "danish",
  "dutch",
  "english",
  "estonian",
  "finnish",
  "french",
  "frisian",
  "galician",
  "german",
  "greek",
  "hungarian",
  "icelandic",
  "irish",
  "italian",
  "latvian",
  "limburgish",
  "lithuanian",
  "macedonian",
  "manx",
  "norwegian",
  "occitan",
  "polish",
  "portuguese",
  "romanian",
  "russian",
  "sardinian",
  "serbian",
  "scottish",
  "slovak",
  "slovene",
  "spanish",
  "swedish",
  "ukrainian",
  "welsh",
  "yiddish",
  "esperanto",
  "hawaiian",
  "maori",
  "native-american",
  "astronomy",
  "history",
  "literature",
  "popular-culture",
  "theology",
  "ancient",
  "medieval"
  "all-biblical",
  "mythology"
)

function Mom-Junction {
  $url = "http://www.momjunction.com/baby-names/"
  mkdir "momj"

  foreach ($names in $namesets) {
    $fname = "momj/$names.tzv"

    "NAMES	MEANING	GENDER" | Out-File -FilePath $fname -Encoding utf8NoBOM

    # first iteration, no number...
    $res = Invoke-WebRequest "$url/$names/"
    Write-Output "$names, 0"

    $html = $res.ParsedHtml
    $rows = $html.getElementsByTagName("tr")

    # $rows[0].childNodes[0]
    for ($i=1; $i -le $rows.length; $i++) {
      $nodes = $rows[$i].childNodes
      if ($nodes.length -gt 1) {
        # Write-Output "$($nodes[0].innerText)`t$($nodes[1].innerText)`t$($nodes[2].innerText)"

        "$($nodes[0].innerText)`t$($nodes[1].innerText)`t$($nodes[2].innerText)" | Out-File -FilePath $fname -Encoding utf8NoBOM -Append
      }
    }

    $idx = 2
    while ($res = Invoke-WebRequest "$url/$names/page/$idx/") {
      Write-Output "$names, $idx"
      $idx++

      $html = $res.ParsedHtml
      $rows = $html.getElementsByTagName("tr")

      # $rows[0].childNodes[0]
      for ($i=1; $i -le $rows.length; $i++) {
        $nodes = $rows[$i].childNodes
        if ($nodes.length -gt 1) {
          # Write-Output "$($nodes[0].innerText)`t$($nodes[1].innerText)`t$($nodes[2].innerText)"

          "$($nodes[0].innerText)`t$($nodes[1].innerText)`t$($nodes[2].innerText)" | Out-File -FilePath $fname -Encoding utf8NoBOM -Append
        }
      }
    }
  }
}

function Behind-the-Name {
  $url = "https://www.behindthename.com/names/usage"
  mkdir "btn"

  foreach ($names in $namesets) {
    $idx = 1
    $fname = "btn/$names.tzv"

    "NAME	SCRIPT	LOCATION	MEANING	GENDER" | Out-File -FilePath $fname -Encoding utf8NoBOM

    while ($res = Invoke-WebRequest "$url/$names/$idx/") {
      Write-Output "$names, $idx"
      $idx++

      $html = $res.ParsedHtml
      $rows = $html.getElementsByClassName("browsename")

      # $rows[0].childNodes[0]
      if ($rows.length -eq 0) {
        break
      }

      Write-Output "Getting $($rows.length) names"
      for ($i=0; $i -lt $rows.length; $i++) {
        $nodes = $rows[$i]

        $ch = [System.Collections.ArrayList]@()

        $ch.Add($nodes.getElementsByClassName("listname")[0]) | Out-Null
        $name = $nodes.getElementsByClassName("listname")[0].innerText # name

        $script = ""
        if ($nodes.getElementsByClassName("listtrans").length -gt 0) {
          $ch.Add($nodes.getElementsByClassName("listtrans")[0]) | Out-Null
          $script = $nodes.getElementsByClassName("listtrans")[0].innerText # orig script
        }

        $ch.Add($nodes.getElementsByClassName("listgender")[0]) | Out-Null
        $gender = $nodes.getElementsByClassName("listgender")[0].innerText # gender

        $ch.Add($nodes.getElementsByClassName("listusage")[0]) | Out-Null
        $location = $nodes.getElementsByClassName("listusage")[0].innerText # location

        # get the line break
        $ch.Add($nodes.getElementsByTagName("br")[0]) | Out-Null

        # Remove nodes
        for ($j = 0; $j -lt $ch.Count; $j++) {
          $nodes.removeChild($ch[$j]) | Out-Null
        }
        $meaning = $nodes.innerText.Trim() # remove top and get meaning

        "$name`t$script`t$location`t$meaning`t$gender" | Out-File -FilePath $fname -Encoding utf8NoBOM -Append

      }
    }
  }
}

Mom-Junction
# Behind-the-Name


# another set to get:
# https://www.familyeducation.com/baby-names/browse-origin/surname

