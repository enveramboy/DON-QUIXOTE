let root;
let menu_loaded = false;

window.addEventListener('load', on_load);

function make_element(type, id, c, p) {
  let ret = document.createElement(type);
  if (id) ret.id = id;
  if (c) ret.className = c;
  p.appendChild(ret);
  return ret;
}

function title_load(r) {
  r.innerHTMl = '';
  let title_div = make_element("div", "title_div", null, r);
  let call_p = make_element("p", "call_p", null, title_div);
  call_p.innerText = "CALL";
  let command_p = make_element("p", "command_p", null, title_div);
  command_p.innerText = "PRESS ANY KEY";
  document.body.addEventListener("keydown", () => { menu_load(r); });
}

function menu_load(r) {
  r.innerHTML = '';
  document.body.style = "background: black;";
  let codec_div = make_element("div", "codec_div", null, r);
  let visual_div = make_element("div", "visual_div", null, codec_div);
  let don_avatar = make_element("img", "don_avatar", "avatar", visual_div);
  don_avatar.src = "images/don_quiet_mono.jpg";
  don_avatar.style.webkitAnimationName = 'load';
  don_avatar.style.webkitAnimationDuration = '0.3s';
  let radio_data = make_element("img", "radio_data", null, visual_div);
  radio_data.src = "images/radio_data.gif";
  let duck_avatar = make_element("img", "duck_avatar", "avatar", visual_div);
  duck_avatar.src = "images/duck_avatar.jpg";
  duck_avatar.style.webkitAnimationName = 'load';
  duck_avatar.style.webkitAnimationDuration = '0.3s';
  let text = make_element("p", "text", null, codec_div);
  let btn_delay;
  if (!menu_loaded) {
    btn_delay = "I AM DON QUIXOTE DE LA MANCHA!STATE YOUR BUSINESS DEAR SQUIRE!".length*100 + 600;
    animate_msg(["I AM DON QUIXOTE DE LA MANCHA!", "STATE YOUR BUSINESS DEAR SQUIRE!"]);
    setTimeout(() => { animate_msg(["MAKE HASTE SQUIRE!", "TO WAVER IN FRONT OF A DREAM IS TO LOSE IT!"]); }, btn_delay + 10000);
  }
  else {
    btn_delay = "WHAT ELSE IS YOUR DESIRE?".length*100 + 600;
    animate_msg(["WHAT ELSE IS YOUR DESIRE?"]);
    setTimeout(() => { animate_msg(["PACE IS OF UTMOST IMPORTANCE, FOR THE DAYLIGHT WANES!"]); }, btn_delay + 10000);
  }
  setTimeout(() => {
    let controls_btn = make_element("button", "controls_btn", "hud_btn", codec_div);
    controls_btn.innerText = "controls";
    controls_btn.addEventListener("click", () => { controls_load(r); });
    let about_btn = make_element("button", "about_btn", "hud_btn", codec_div);
    about_btn.innerText = "about";
    about_btn.addEventListener("click", () => { about_load(r); });
  }, btn_delay);

  menu_loaded = true;

  function animate_sentence(s) {
    for (let i = 0; i < 2*(s.split(" ").length); i++) {
      if (i % 2 === 0) setTimeout(() => { don_avatar.src = "images/don_talk_mono.png"; }, 200*i);
      else setTimeout(() => { don_avatar.src = "images/don_quiet_mono.jpg"; }, 200*i);
    }
    text.innerText = "";
    let buffer = "";
    s.split("").forEach((l, i) => {
      setTimeout(() => { 
        buffer = buffer.concat(l);
        text.innerText = buffer;
       }, i*100);
    });
  }

  function animate_msg(msg) {
    let timeout = 0;
    msg.forEach((s) => {
      setTimeout(() => { animate_sentence(s); }, timeout);
      timeout += s.length*100 + 500;
    });
  }

}

function controls_load(r) {
  r.innerHTML = '';
  document.body.addEventListener("keydown", () => { menu_load(r); });
  let return_p = make_element("p", "return_p", null, r);
  return_p.innerText = "PRESS ANY KEY TO CONFIRM";
  let controls_div = make_element("div", "controls_div", null, r);
  let diagram_div = make_element("div", "diagram_div", null, controls_div);
  let front_p = make_element("p", "front_p", null, diagram_div);
  front_p.innerText = "FRONT";
  let diagram_img = make_element("img", "diagram_img", null, diagram_div);
  diagram_img.src = "images/don_quixote_top_view.jpg";
  let rear_p = make_element("p", "rear_p", null, diagram_div);
  rear_p.innerText = "REAR";
  let controls = make_element("div", "controls", null, controls_div);
  ["KEYBOARD", "ACTION"].forEach(c => { 
    let p = make_element("p", "header_p", null, controls); 
    p.innerText = c;
  });
  ["W", "FORWARD", "A", "LEFT", "S", "REVERSE", "D", "RIGHT", "SHIFT", "SPRINT"].forEach(c => { 
    let p = make_element("p", "controls_p", null, controls); 
    p.innerText = c;
  });
}

function about_load(r) {
  r.innerHTML = "";
  document.body.style = 'background: url("images/about_bg.png") no-repeat center center fixed; webkit-background-size: cover; -o-background-size: cover; background-size: cover;';
  document.body.addEventListener("keydown", () => { menu_load(r); });
  let return_p = make_element("p", "return_p", null, r);
  return_p.innerText = "PRESS ANY KEY TO CONFIRM";
  let about_container = make_element("div", "about_container", null, r);
  let slide_container = make_element("div", "slide_container", null, about_container);
  let slide_tag = make_element("div", "slide_tag", null, slide_container);
  slide_tag.innerText = "INFO";
  let slide_triangle = make_element("div", "slide_triangle", null, slide_container);
  let info_container = make_element("div", "info_container", null, about_container);

  let info = make_element("p", "info", null, info_container);
  let about_img = make_element("div", "about_img", null, info_container);
  let img = make_element("img", "img", null, about_img);
  let backward = make_element("button", "backward", "view_button", about_container);
  backward.innerText = "<";
  let forward = make_element("button", "forward", "view_button", about_container);
  forward.innerText = ">"
  let slides = [() => { 
      animate_msg(["Don Quixote is a remote controlled Strandbeest utilizing Griddlock’s STL design (https://www.thingiverse.com/thing:4030188)."]); 
      about_img.innerHTML = "";
      img = make_element("img", "img", null, about_img);
      img.src = "images/slide_1.gif"; 
    }, 
    () => { animate_msg(["It integrates a websocket control system, allowing WiFi control via website ran by an ESP32. Keypresses are read and displayed on screen, then corresponding commands are sent to the ESP32."]); img.src = "images/slide_2.gif"; },
    () => { 
      animate_msg(["The circuit utilizes an L298N motor driver to allow the ESP32 to drive two DC motors forwards and backwards. These motors in turn drive the legs of the Strandbeest. Power for the circuit is supplied via a 11.1V LiPo battery."]); 
      about_img.innerHTML = "";
      img = make_element("img", "img", null, about_img);
      img.src = "images/slide_3.png"; 
    },
    () => { 
      animate_msg(["A provided performance video:"]); 
      about_img.innerHTML = '<video width="300" height="240" controls><source src="images/slide_4.mp4" type="video/mp4">The browser does not support videos!</video>';
    }
  ];

  let index = 0;

  slides[index]();

  backward.addEventListener("click", () => {
    let max = setTimeout(() => {}, 0);
    while (max >= 0) { 
      window.clearTimeout(max);
      max--;
    }
    index = (index > 0) ? index - 1 : slides.length - 1;
    slides[index]();
  });

  forward.addEventListener("click", () => {
    let max = setTimeout(() => {}, 0);
    while (max >= 0) { 
      window.clearTimeout(max);
      max--;
    }
    index = (index < slides.length - 1) ? index + 1 : 0;
    slides[index]();
  });

  function animate_sentence(s) {
    info.innerText = "";
    let buffer = "";
    s.split("").forEach((l, i) => {
      setTimeout(() => { 
        buffer = buffer.concat(l);
        info.innerText = buffer;
       }, i*25);
    });
  }

  function animate_msg(msg) {
    let timeout = 0;
    msg.forEach((s) => {
      setTimeout(() => { animate_sentence(s); }, timeout);
      timeout += s.length*100 + 500;
    });
  }
}

function on_load(event) {
  root = document.getElementById("root");
  title_load(root);
}