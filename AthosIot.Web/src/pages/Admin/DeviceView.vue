<template>
  <card>
    <h4 slot="header" class="card-title">{{device.name}} <div class="float-right small">({{device.deviceid}} - {{device.type}} - {{device.version}})</div></h4>
    <div class="row">
      <div class="col-lg-12">
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="wipe">
          Wipe
        </button>
        <button type="submit" class="btn btn-light float-right" @click.prevent="reset">
          Reset
        </button>
        <button type="submit" class="btn btn-secondary  float-right" @click.prevent="rename">
          Rename
        </button>
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="ping">
          Ping
        </button>        
        <div v-if="device.last.type == 'RELAY'">
            <button type="submit" class="btn btn-secondary  float-right" @click.prevent="all_off(device)">
              All Off
            </button>
            <button type="submit" class="btn btn-secondary  float-right" @click.prevent="all_on(device)">
              All On
            </button>
        </div>
      </div>
    </div>
    <div class="row">
      <div class="col-md-12">
        <div v-if="device.last">
          <div v-if="device.last.type == 'BMP280'">
              Temperature: {{device.last.temp}}&deg;C<br/>
              Humidity: {{device.last.humidity}}<br/>
              Pressure: {{device.last.pressure}}<br/>
              Altitude: {{device.last.altitude}}<br/>
          </div>          
          <div v-if="device.last.type == 'TMP36'">
              Temperature: {{device.last.temp}}&deg;C<br/>
          </div>          
          <div v-if="device.last.type == 'DHT11'">
              Temperature: {{device.last.temp}}&deg;C<br/>
              Humidity: {{device.last.humidity}}<br/>
              Heat Index: {{device.last.pressure}}<br/>
          </div>          
          <div v-if="device.last.type == 'PIR'">
              Movement: {{device.last_motion.timeStamp}}&deg;C<br/>
              State: {{device.last_motion.movement}}<br/>
          </div>          
          <div v-if="device.last.type == 'RELAY'">
            <ul list-group class="list-group">
              <li v-for="o in device.relay" class="list-group-item list-group-item-action flex-column align-items-start">
                <div v-if="o.id">
                  {{o.id}} : {{o.name}}
                  <button type="submit" class="btn btn-secondary  float-right" @click.prevent="relay_rename(device, o)">
                    Rename
                  </button>
                  <button type="submit" class="btn btn-secondary  float-right" @click.prevent="relay_off(device, o)">
                    Off
                  </button>
                  <button type="submit" class="btn btn-secondary  float-right" @click.prevent="relay_on(device, o)">
                    On
                  </button>
                </div>          
              </li>
            </ul>
          </div>          
          {{device.last.timeStamp}}
          <div v-if="device.ping">
            Ping Delay:{{device.ping.pongDiff}}            
          </div>    
        </div>          
      </div>
    </div>
    <div class="clearfix"></div>
  </card>
</template>
<script>
import Card from 'src/components/Cards/Card.vue'
import Vue from 'vue'

export default {
    components: {
      Card
    },
    props : ['device'],
    data () {
      return {
      }
    },
      mounted() {    
        // var vm = this;    
        // var sock = Vue.prototype.$socket;
        // sock.send(JSON.stringify({ action : "get-wifi" }));

        // sock.onmessage = function(event) {
        //   if(event && event.data) {
        //     var msg = JSON.parse(event.data);
        //     if(msg.action && msg.action === "update-wifi") {
        //       if(msg.ssid) vm.wifi.ssid = msg.ssid;
        //       if(msg.password) vm.wifi.password = msg.password;
        //     }
        //   }
        // };
      },   
      methods: {
        all_on(device) {
          console.log('all_on:',  device.deviceid);
          Vue.prototype.$socket.send(JSON.stringify({ action : "all-on", deviceid : device.deviceid }));

        },
        all_off(device) {
          console.log('all_off:',  device.deviceid);
          Vue.prototype.$socket.send(JSON.stringify({ action : "all-off", deviceid : device.deviceid }));
        },
        relay_rename(device, relay){
          console.log('rename relay:',  device.deviceid, relay.id);
          //rename-relay-node
          var existingName = "My Relay Switch";

          if(relay.name && relay.name !== "") {
             existingName = relay.name;
          }
          var name = window.prompt("Rename Relay Switch" + this.device.deviceid + " ("+relay.id+")", existingName);
          if(name) {
            console.log('renaming relay switch:', relay.id, name );
            relay.name = name;
            Vue.prototype.$socket.send(JSON.stringify({ action : "rename-relay-switch", device, relay, name }));
          }
        },
        relay_on(device, relay){
          console.log('on relay:',  device.deviceid, relay.id);
          Vue.prototype.$socket.send(JSON.stringify({ action : "relay-on", deviceid : device.deviceid, relay : relay.id }));
        },
        relay_off(device, relay){
          console.log('off relay:',  device.deviceid, relay.id);
          Vue.prototype.$socket.send(JSON.stringify({ action : "relay-off", deviceid : device.deviceid, relay : relay.id }));
        },
        ping() {
          console.log('pinging device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "ping-device", ts: new Date(), deviceid : this.device.deviceid }));
        }, 
        rename() {
          var existingName = "My Device";
          if(this.device.name && this.device.name !== "") {
             existingName = this.device.name;
          }
          var name = window.prompt("Rename device" + this.device.deviceid,existingName);
          if(name) {
            console.log('renaming device:', this.device.deviceid, name );
            Vue.prototype.$socket.send(JSON.stringify({ action : "rename-device", deviceid : this.device.deviceid, name }));
          }
        }, 
        wipe() {
          console.log('wiping device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "wipe-device", deviceid : this.device.deviceid }));
        }, 
        reset() {
          console.log('reset device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "reset-device", deviceid : this.device.deviceid }));
        },
        reboot() {
          console.log('reboot device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "reboot-device", deviceid : this.device.deviceid }));
        }
    }
  }

</script>
<style>

</style>
