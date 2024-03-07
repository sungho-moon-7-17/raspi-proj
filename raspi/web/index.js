document.querySelector(".gpio-4-on").onclick = () => {button_click(4, 1)};
document.querySelector(".gpio-4-off").onclick = () => {button_click(4, 0)};

function button_click(pinNum, pinState){
  console.log("button click")
  
  fetch('http://192.168.219.108:8080', {
      method: 'PUT',
      headers: {
        'Content-Type': 'application/json',
      },
      body: `${pinNum} ${pinState}`,
    })
}


