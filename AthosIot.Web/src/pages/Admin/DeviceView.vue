<template>
  <card>
    <h4 slot="header" class="card-title">{{device.name}} <div class="float-right">({{device.deviceid}})</div></h4>
    <div class="row">
      <div class="col-md-12">
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="wipe">
          Wipe
        </button>
        &nbsp;
        <button type="submit" class="btn btn-light float-right" @click.prevent="reset">
          Reset
        </button>
        &nbsp;
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="rename">
          Rename
        </button>
      </div>
    </div>
    <div class="row">
      <div class="col-md-12">
        <div v-if="device.last">
          Device: {{device.last.type}} ({{device.last.version}})<br/>
          <div v-if="device.last.type == 'BMP280'">
              Temperature: {{device.last.temp}}&deg;C<br/>
              Humidity: {{device.last.humidity}}<br/>
              Pressure: {{device.last.pressure}}<br/>
          </div>          
          <div v-if="device.last.type == 'TMP36'">
              Temperature: {{device.last.temp}}&deg;C<br/>
              Humidity: {{device.last.humidity}}<br/>
              Pressure: {{device.last.pressure}}<br/>
          </div>          
          {{device.last.timeStamp}}
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
        rename() {
          var name = window.prompt("Rename device" + this.device.deviceid,"My Device");
          console.log('renaming device:', this.device.deviceid, name );
          Vue.prototype.$socket.send(JSON.stringify({ action : "rename-device", deviceid : this.device.deviceid, name }));
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
