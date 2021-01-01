#ifndef HTML_H
#define HTML_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Kitchen Led</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action = "/get" onsubmit="onSubmit();">

  <div id="div_mode">
    Mode: <select name="mode" id="mode" onchange="updateMode(); return false;">
      <option value="off">Off</option>
      <option value="simple">Simple</option>
      <option value="rainbow">Rainbow</option>
      <option value="rainbowfade">Rainbow Fade</option>
    </select><br/>
    
    <div id="mode_settings">
    <div id="colorDiv">
    Color: <input type="color" id="color" name="color" value="#e66465"><br/>
    </div>
      Speed: <input type="range" name="mode_speed" id="mode_speed" min="0" max="100" value="50"
              onchange="updateSpeedLabel(this)" oninput="updateSpeedLabel(this)">
      <label id = "label_mode_speed">50</label><br/>
      Inverted: <input type="checkbox" name="mode_inverted" id="mode_inverted" value = '1'><br/>
    <input type='hidden' name='mode_inverted' id='mode_inverted_hidden' value='0' >
    </div>
  </div>

  <div id="div_mask">
  <br/>
    Mask: <select name="mask" id="mask">
      <option value="full">Full</option>
      <option value="grownback">Grow & Back</option>
      <option value="grownback2center">Grow & Back 2 center</option>
    </select><br/>
    
    <div id="mask_settings">
      Speed: <input type="range" name="mask_speed" id="mask_speed" min="0" max="100" value="50"
              onchange="updateSpeedLabel(this)" oninput="updateSpeedLabel(this)">
      <label id = "label_mask_speed">50</label><br/>
      Inverted: <input type="checkbox" name="mask_inverted" id="mask_inverted" value='1'><br/>
    <input type='hidden' name='mask_inverted' id='mask_inverted_hidden' value='0' >
    </div>
  </div>
  
  <div id="settings">
  <br/>
    Brightness: <input type="range" name="brightness"  id="brightness" min="0" max="255" value="75"><br/>
  </div>
  
  <input type="submit" value="Set"><br/>
  
  <script type="text/javascript">

    function onSubmit()
    {
      document.getElementById('mode_inverted_hidden').disabled = document.getElementById("mode_inverted").checked;
    }

    function updateSpeedLabel(element)
    {
      document.getElementById('label_'+element.id).innerHTML = element.value;
    }
  
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

      var off = mode=='off';
      showStuff('mode_settings', !off);
      showStuff('div_mask', !off);
      showStuff('settings', !off);
    }

    var urlParams;

    function setValueFromUrlParams(id, def)
    {
      document.getElementById(id).value = urlParams.get(id) || def;
    }
  
    function setCheckedFromUrlParams(id, def)
    {
      let element = document.getElementById(id);
      element.checked = urlParams.get(id) && urlParams.get(id) == element.value || def;
    }
  
    function load()
    {
      urlParams = new URLSearchParams(window.location.search);
      
      setValueFromUrlParams('mode', 'off');
      setValueFromUrlParams('mask', 'full');
      setValueFromUrlParams('color', '#ff0000');
      setValueFromUrlParams('brightness', '75');
      setValueFromUrlParams('mode_speed', '50');
      setValueFromUrlParams('mask_speed', '50');
      setCheckedFromUrlParams('mode_inverted', false);
      setCheckedFromUrlParams('mask_inverted', false);
      
      updateMode();
    }
    
    window.onload = load;
  </script>
  
  </form><br>
</body>
</html>
)rawliteral";


#endif // HTML_H
