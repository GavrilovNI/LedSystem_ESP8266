#ifndef HTML_H
#define HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Kitchen Led</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    
  
  Mode: <select name="mode" id="mode" onchange="updateMode(); return false;">
    <option value="off">Off</option>
    <option value="simple">Simple</option>
    <option value="rainbow">Rainbow</option>
    <option value="rainbowfade">Rainbow Fade</option>
  </select><br/>

  Mask: <select name="mask" id="mask">
    <option value="full">Full</option>
    <option value="grownback">Grow & Back</option>
    <option value="grownback2center">Grow & Back 2 center</option>
  </select><br/>
  
  <div id="colorDiv">
    Color: <input type="color" id="color" name="color" value="#e66465"><br/>
  </div>
  <div id="on">
    Brightness: <input type="range" name="brightness"  id="brightness" min="0" max="255" value="75"><br/>
    Speed: <input type="range" name="speed"  id="speed" min="-100" max="100" value="50"><br/>
  </div>
    <input type="submit" value="Set"><br/>

    <script type="text/javascript">
  
    function showStuff(id, value)
      {
        document.getElementById(id).style.display = value==true?'block':'none';
      }
  
    function updateMode()
      {
        const mode = document.getElementById('mode').value;
        var coloredModes = ['simple'];
        var showColor = false;
        coloredModes.forEach(function(item, index, array) {
          if(mode==item)
            showColor=true;
        });
        showStuff('colorDiv', showColor);
        showStuff('on', mode!='off');
      }
  
      function codeAddress()
      {
        const defaultColor = '#ff0000';
        const queryString = window.location.search;
        
        const urlParams = new URLSearchParams(queryString);
        document.getElementById('mode').value = urlParams.get('mode') || 'off';
        document.getElementById('mask').value = urlParams.get('mask') || 'full';
        document.getElementById('color').value = urlParams.get('color') || defaultColor;
        document.getElementById('brightness').value = urlParams.get('brightness') || '75';
        document.getElementById('speed').value = urlParams.get('speed') || '50';
        
        updateMode();
      }
      window.onload = codeAddress;
    </script>
  </form><br>
</body>
</html>
)rawliteral";

#endif // HTML_H
