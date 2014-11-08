struct pattern {
        int one;
        int two;
        int three;
        int four;
        int five;
};

int timer = 0;
struct pattern pattern1;
struct pattern pattern2;
struct pattern pattern3;
struct pattern patterns[3];
int num_patterns;
int removed;
int num_elements;
int start_time;
int duration;

int led = 13;

void setup() {
        struct pattern pattern1 = {1500, 500, 500, NULL, NULL};
        struct pattern pattern2 = {1000, 1500, 500, 500, 500};
        struct pattern pattern3 = {1500, 1000, 500, NULL, NULL};
        struct pattern patterns[] = {pattern1, pattern2, pattern3};
        num_patterns = 3;
        removed = 0;
        num_elements = 0;
        start_time = 0;
        duration = 0;
        pinMode(led, OUTPUT);
        Serial.begin(9600);
}

long get_max(int action_num) {
        int max = 0;
        int* numPtr;
        for (int i = 0; i < num_patterns - removed; i++) {
                struct pattern curr = patterns[i];
                numPtr = (int*)&curr;
                if (*(numPtr + action_num - 1) > max)
                        max = *(numPtr + action_num - 1);
        }
        return max;
}

void select_timers(int window) {
        int* numPtr;
        for (int i = 0; i < num_patterns - removed; i++) {
                struct pattern curr = patterns[i];
                numPtr = (int*)&curr;
                if (*(numPtr + timer - 1) != window) {
                        remove_pattern(i);
                }
        }
}

void remove_pattern(int loc) {
        int* numPtr = (int*)&patterns[0];
        *(numPtr + loc) = NULL;
        struct pattern curr = patterns[loc+1];
        numPtr = (int*)&curr;
        while(*(numPtr + loc + 1) != NULL) {
                patterns[loc] = curr;
                loc++;
                curr = patterns[loc+1];
        }
        num_patterns--;
}

int get_num_elements(int loc) {
        int curr_elt = patterns[loc].one;
        int elts = 1;
        while (curr_elt != NULL) {
                if (curr_elt == patterns[loc].one)
                        curr_elt = patterns[loc].two;
                else if (curr_elt == patterns[loc].two)
                        curr_elt = patterns[loc].three;
                else if (curr_elt == patterns[loc].three)
                        curr_elt = patterns[loc].four;
                else if (curr_elt == patterns[loc].four)
                        curr_elt = patterns[loc].five;
                else
                        curr_elt = NULL;
                elts++;
        }
        return elts;
}

void loop() {
        int sensorValue = analogRead(A0);
        if (timer == 0){
                if(sensorValue > 0) {
                        start_time = millis();
                        duration = get_max(0);
                        timer++;
                }
        }
        if(sensorValue > 0) {
                int window = (start_time + duration - millis());
                if (window > 400 && window < 600)
                        window = 500;
                else if (window > 900 && window < 1100)
                        window = 1000;
                else if (window > 1400 && window < 1600)
                        window = 1500;
                else if (window > 1900 && window < 2100)
                        window = 2000;
                else
                        setup();
                if (timer > 0) {
                        select_timers(window);
                        start_time = millis();
                        duration = get_max(timer);
                        timer++;
                }
        }
        if (millis() > start_time + duration) {
                        for (int i = 0; i < num_patterns; i++) {
                                    if (get_num_elements(i) < timer) {
                                                digitalWrite(led, 1);
                                    }
                        }
                        setup();
        }
        if (num_patterns == 1 && timer > get_num_elements(0)) {
                        digitalWrite(led, 1);
        }
        Serial.println(sensorValue);
        delay(1);
}


