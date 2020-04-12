<template>
  <card>
    <h3 slot="header" class="card-title">
      {{device.name}} 
      <div class="float-right small" :title="tooltip(device)">
          ({{device.deviceid}} - {{device.type}} - {{device.version}})
      </div>
    </h3>    
    <hr width="80%" />
    <div class="row">
      <div class="col-lg-12">
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="wipe">
          Wipe
        </button>
        <button type="submit" class="btn btn-light float-right" @click.prevent="reset">
          Reset
        </button>
        <button type="submit" class="btn btn-secondary  float-right" @click.prevent="rename(device)">
          Rename
        </button>
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="firmware_update(device)">
          Firmware
        </button>    
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="delete_device(device)">
          Delete
        </button>                   
      </div>
    </div>
    <div class="row">
      <div class="col-lg-12">
        <button type="submit" class="btn btn-secondary float-right" @click.prevent="ping">
          Ping
        </button>        
        <div v-if="device.relay">
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
      <div class="col-lg-12">
        <div class="table-responsive">
          <table class="table table-hover">
              <tbody>
                <div v-if="device.bmp280">
                  <tr class="row">
                    <th scope="row" class="col-lg-4">Temperature</th>
                    <td class="col-lg-8">{{device.bmp280.temp}}&deg;C</td>
                  </tr>
                  <tr class="row">
                    <th scope="row" class="col-lg-4">Humidity</th>
                    <td class="col-lg-8">{{device.bmp280.humidity}}% Relative Humidity</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Pressure</th>
                    <td class="col-lg-8">{{device.bmp280.pressure}} hPa</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Altitude</th>
                    <td class="col-lg-8">{{device.bmp280.altitude}} meters</td>
                  </tr>
                </div>
                <div v-if="device.tmp36">
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Temperature</th>
                    <td class="col-lg-8">{{device.tmp36.temp}}&deg;C</td>
                  </tr>                  
                </div>
                <div v-if="device.dht11">
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Temperature</th>
                    <td class="col-lg-8">{{device.dht11.temp}}&deg;C</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Humidity</th>
                    <td class="col-lg-8">{{device.dht11.humidity}} % Relative Humidity</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Heat Index</th>
                    <td class="col-lg-8">{{device.dht11.headIndex}}&deg;C</td>
                  </tr>
                </div>          
                <div v-if="device.moisture">
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Moisture Level</th>
                    <td class="col-lg-8">{{device.moisture.level}}%</td>
                  </tr>
                </div>               
                <div v-if="device.mq2">
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Ratio</th>
                    <td class="col-lg-8">{{device.mq2.ratio}}%</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">LPG</th>
                    <td class="col-lg-8">{{device.mq2.lpg}} ppm</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Methane</th>
                    <td class="col-lg-8">{{device.mq2.methane}} ppm</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Smoke</th>
                    <td class="col-lg-8">{{device.mq2.smoke}} ppm</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Hydrogen</th>
                    <td class="col-lg-8">{{device.mq2.hydrogen}} ppm</td>
                  </tr>
                </div>               
                <div v-if="device.pir">
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">Last Movement</th>
                    <td class="col-lg-8">{{(new Date(device.pir.movement.timeStamp)).toLocaleString()}}</td>
                  </tr>
                  <tr  class="row">
                    <th scope="row" class="col-lg-4">State</th>
                    <td class="col-lg-8">{{device.pir.movement.moved}}</td>
                  </tr>
                </div>               
                <div v-if="device.relay">
                    <tr class="row" v-for="o in device.relay"  v-if="o.id" >
                      <th scope="row" class="col-lg-4">{{o.id}} : {{o.name}}</th>
                      <td class="float-right col-lg-8">
                        <button type="submit" class="btn btn-secondary text-xs-right" @click="relay_rename(device, o)">
                        Rename
                        </button>
                        <div v-if="!o.id.startsWith('M')">
                          <button type="submit" class="btn btn-secondary" @click.prevent="relay_off(device, o)">
                            Off
                          </button>
                          <button type="submit" class="btn btn-secondary" @click.prevent="relay_on(device, o)">
                            On
                          </button>        
                        </div>             
                        <div v-if="o.id.startsWith('M')">
                          <button type="submit" class="btn btn-secondary" @click.prevent="relay_off(device, o)">
                            Toggle
                          </button>
                        </div>             
                      </td>
                    </tr>
                </div>
                <tr  class="row">
                  <th scope="row" class="col-lg-4">Ping Delay</th>
                  <td class="col-lg-8"><div v-if="device.ping">{{device.ping.roundTrip}} seconds</div></td>
                </tr>
                <tr  class="row">
                  <th scope="row" class="col-lg-4">Last Updated</th>
                  <td class="col-lg-8">{{(new Date(device.timeStamp)).toLocaleString()}}</td>
                </tr>
              </tbody>
            </table>
          </div>
      </div>
    </div>
    <div class="clearfix"></div>
    <rename-relay-modal name="rename-relay-modal"/>
    <rename-device-modal name="rename-device-modal"/>
    <device-firmware-update-modal name="device-firmware-update-modal"/>

  </card>  

</template>
<script>
import Card from 'src/components/Cards/Card.vue'
import Vue from 'vue'
import RenameRelayModal from 'src/components/RenameRelayModal.vue'
import RenameDeviceModal from 'src/components/RenameDeviceModal.vue'
import DeviceFirmwareUpdateModal from 'src/components/DeviceFirmwareUpdateModal.vue'

export default {
    components: {
      Card,
      RenameRelayModal,
      RenameDeviceModal,
      DeviceFirmwareUpdateModal
    },
    props : ['device'],
    data () {
      return {
      }
    },
    computed : {
    },
      mounted() {    
      },   
      methods: {
        tooltip(device) {
          var tip =                                    "Name:                        " + device.name + "\n";
          tip +=                                       "Version:                     " + device.version + "\n";
          tip +=                                       "Device Id:                   " + device.deviceid + "\n";
          if(device.system) {
            if(device.system.chipId) tip +=            "Chip Id:                      " + device.system.chipId + "\n";
            if(device.system.cpuFreqMHz) tip +=        "CPU Freq MHz:           " + device.system.cpuFreqMHz + "\n";
            if(device.system.flashChipVendorId) tip += "Flash Chip Vendor Id: " + device.system.flashChipVendorId + "\n";
            if(device.system.freeSketchSpace) tip +=   "Free Sketch Space:     " + device.system.freeSketchSpace + "\n";
            if(device.system.sketchMD5) tip +=         "Sketch MD5:               " + device.system.sketchMD5 + "\n";
            if(device.system.vcc) tip +=               "Vcc:                             " + device.system.vcc + "\n";
            if(device.system.localIP) tip +=           "Local IP:                     " + device.system.localIP + "\n";
            if(device.system.macAddress) tip +=        "Mac Address:             " + device.system.macAddress + "\n";
            if(device.system.gatewayIP) tip +=         "Gateway IP:               " + device.system.gatewayIP + "\n";
            if(device.system.SSID) tip +=              "SSID:                           " + device.system.SSID + "\n";
            if(device.system.coreVersion) tip +=       "Core Version:             " + device.system.coreVersion + "\n";
            if(device.system.sdkVersion) tip +=        "SDK Version:              " + device.system.sdkVersion + "\n";
            if(device.system.rssi) tip +=              "RSSI:                          " + device.system.rssi + "dB\n";
            if(device.system.rssi_strength) {
              if(device.system.rssi_strength.title) tip += "\nThis node's Signal Stregth is " + device.system.rssi_strength.title + "\n";
              if(device.system.rssi_strength.desc) tip +=  "\n" + device.system.rssi_strength.desc + "\n";
            }

          }
          console.log(tip);          
          return tip;
        },
        all_on(device) {
          console.log('all_on:',  device.deviceid);
          Vue.prototype.$socket.send(JSON.stringify({ action : "all-on", deviceid : device.deviceid }));

        },
        all_off(device) {
          console.log('all_off:',  device.deviceid);
          Vue.prototype.$socket.send(JSON.stringify({ action : "all-off", deviceid : device.deviceid }));
        },
        firmware_update(device) {
          console.log('firmware_update:',  device.deviceid);

          this.$modal.show('device-firmware-update-modal', { model : {
            device : device
          }});
        },
        relay_rename(device, relay){
          console.log('rename relay:',  device.deviceid, relay.id);

          this.$modal.show('rename-relay-modal', { model : {
            relay : relay,
            device : device,
          }});
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
        rename(device) {
          console.log('rename device:', this.device.deviceid );
          this.$modal.show('rename-device-modal', { model : { device : device }});
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
        },
        delete_device() {
          console.log('delete-device:', this.device.deviceid );
          Vue.prototype.$socket.send(JSON.stringify({ action : "delete-device", deviceid : this.device.deviceid }));
        }
    }
  }

</script>
<style>

</style>
