<template>
    <div class="content">
    <card class="card-firmware">
    <h4 slot="header" class="card-title">Manage Firmware</h4>
        <div class="container-fluid">
            <div class="row">
                <div class="col-md-12">
                    <ul list-group class="list-group">
                        <li v-for="o in firmware"
                            class="list-group-item list-group-item-action flex-column align-items-start">
                            {{o}}&nbsp;&nbsp;
                            <button type="submit" class="btn btn-secondary  float-right" @click="firmware_delete(o)">
                                Delete
                            </button>
                        </li>
                    </ul>
                </div>
              </div>
        </div>
    </card>
    <card class="card-firmware-upload">
    <h4 slot="header" class="card-title">Upload Firmware</h4>
        <div class="container-fluid">
                <div class="row">
                    <div class="col-md-10">
                        Upload New Firmware
                    </div>
                    <div class="col-md-2">
                        <file-upload :url='url' accept=".bin" @change="onFileChange"></file-upload>
                    </div>
                </div>
              </div>
    </card>
  </div>
</template>
<script>
  import DeviceView from './Admin/DeviceView.vue'
  import Vue from 'vue'

  export default {
    components: {
      DeviceView
    },
      data() {
        return {
          firmware : [],
          url: 'http://'+window.location.hostname+':1880/upload',
          filesUploaded: []          
        }
    },
    mounted() {    
      var vm = this;    

      var sock = Vue.prototype.$socket;
      sock.send(JSON.stringify({ action : "get-firmware" }));
      sock.onmessage = function(event) {          
        if(event && event.data) {
          var msg = JSON.parse(event.data);

          if(msg.action && msg.action === "firmware-list") {
            vm.firmware = msg.list;
            console.log(this.firmware);        
          }
        }
      };
    },   
    methods: {
      firmware_delete(path) {        
        Vue.prototype.$socket.send(JSON.stringify({ action : "delete-firmware", path }));
      },
      onFileChange(file) {
        this.fileUploaded = file
        console.log("onFileChange", file);
      }
    }
  }

</script>
<style>

</style>
