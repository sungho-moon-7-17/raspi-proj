let sequence = true; //서로의 턴을 위한 변수
let index_click = 0;
let reference = 0;
$result = document.querySelector("#result");
let selec_pattern = {
    X: [],
    O: []
};
//버튼태그를 집어넣을 배열
let $button_arr = new Array(10);
let clickable = true;


function reset_game() {
    for (let i = 1; i < 10; i++) {
        $button_arr[i] = document.querySelector(`.b-${i}`);//버튼?�� 초기?��
        document.querySelector(`.b-${i}`).onclick = () => { button_click(i) };
        document.querySelector(`.b-${i}`).style.backgroundImage = "none";
    }
    selec_pattern.X = [];
    selec_pattern.O = [];
    index_click = 0;
    $result.textContent = "result";
    sequence = true;
}
reset_game();


function button_click(number) {
    if(!clickable)return;
    $button = document.querySelector(`.b-${number}`);
    $button.onclick = () => { };
    index_click++;
    if (reference === number)
        reference = 0;
    if (sequence) {
        $button.style.backgroundImage = `url("./X.png")`
        $button.style.backgroundSize = "100%";
        sequence = false;
        selec_pattern.X.push(number);
    }
    else {
        $button.style.backgroundImage = `url("./O.png")`
        $button.style.backgroundSize = "100% 100%";
        sequence = true;
        selec_pattern.O.push(number);
    }
    end_check();
}


function end_check() {//끝인지 확인하는 함수
    let match_result;
    if (sequence)
        match_result = first(selec_pattern.O);
    else
        match_result = first(selec_pattern.X);

    if (match_result === 10) {
        end_game();
        if (sequence) {
            $result.textContent = "O의 승리입니다."
        }
        else {
            $result.textContent = "X의 승리입니다.";
        }
    }
    else if (index_click === 9) {
        $result.textContent = "무승부입니다.";
        return;
    }
    else if (!sequence) {
        opp_turn(match_result);
    }
}

function first(pattern) {
    let b_num; // 버튼 넘버
    let kang = 0;
    for (let i of [[1, 1, 3, 4], [2, 3], [3, 2, 3], [4, 1], [7, 1], [9, -1, -3, -4], [8, -3], [6, -1]]) { // [버튼?��?��,?��?��?��]
        if (!pattern.includes(b_num = i.shift())) {
            continue;
        }
        for (let j of i) {
            pattern.includes(b_num + j) ? kang++ : 0;
            pattern.includes(b_num + j * 2) ? kang += 10 : 0;
            if (kang == 11) {
                return 10;
            }
            //opp-turn을 구현하기 위한 구문(2개라 막아야 하거나 공격하는거)
            else if (!reference) {
                let temp;
                (kang == 1)?temp = op_tu_re(b_num+j*2):(kang == 10)?temp =op_tu_re(b_num+j):0;
                if(temp){
                    reference = temp;
                }
            }
            kang = 0;
        }
    }
    return reference;
}

function end_game() {
    for (let i = 1; i < 10; i++) {
        document.querySelector(`.b-${i}`).onclick = () => { };
    }
}


function opp_turn(j) { // 상대턴 구현
    clickable = false;
    setTimeout(()=>{
        clickable = true;   
        if (j) {
            $button_arr[j].click();
        }
        else if ($button_arr[5].style.backgroundImage === "none") {
            $button_arr[5].click();
        }
        else{
            for (let i of [1, 3, 7, 9, 2, 4, 6, 8]) {
                if ($button_arr[i].style.backgroundImage === "none") {
                    $button_arr[i].click();
                    break;
                }
            }
        }
    },1000);
}

function op_tu_re(j) {//reference를 위한 함수
    if ($button_arr[j].style.backgroundImage === "none")
        return j;
    return 0;
}